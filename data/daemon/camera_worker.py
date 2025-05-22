import cv2
import numpy as np
import time
import threading
import sqlite3
import datetime
from tracker import CentroidTracker
import os

DB_PATH = os.path.join(os.path.dirname(__file__), "../attendance.db")

def get_current_pair(now=None):
    if now is None:
        now = datetime.datetime.now()

    adjusted_schedule = [
        (datetime.time(7, 50), datetime.time(9, 35)),
        (datetime.time(9, 40), datetime.time(11, 25)),
        (datetime.time(11, 30), datetime.time(13, 15)),
        (datetime.time(13, 35), datetime.time(15, 20)),
        (datetime.time(15, 25), datetime.time(17, 10)),
        (datetime.time(17, 15), datetime.time(19, 0)),
    ]

    for i, (start, end) in enumerate(adjusted_schedule, start=1):
        if start <= now.time() <= end:
            return i
    return None

class CameraWorker:
    def __init__(self, camera_id, url, line_y_ratio, visual=False):
        self.camera_id = camera_id
        self.url = url
        self.line_y_ratio = line_y_ratio
        self.visual = visual

        self.entry_count = 0
        self.exit_count = 0
        self.cross_state = {}

        self.tracker = CentroidTracker(max_disappeared=40, max_distance=75)
        self.hog = cv2.HOGDescriptor()
        self.hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())

        self.latest_boxes = []
        self.latest_frame = None
        self.detection_lock = threading.Lock()
        self.running = True

        self.current_pair = None
        self.current_date = None

    def detection_loop(self):
        while self.running:
            time.sleep(0.3)
            with self.detection_lock:
                if self.latest_frame is None:
                    continue
                frame = self.latest_frame.copy()

            rects, weights = self.hog.detectMultiScale(
                frame, winStride=(8, 8), padding=(16, 16), scale=1.05
            )

            filtered = []
            for (x, y, w, h), conf in zip(rects, weights):
                if conf < 0.6 or h < 1.5 * w or w * h < 5000:
                    continue
                filtered.append((x, y, w, h))

            self.latest_boxes = filtered

    def ensure_pair_visit_exists(self, pair_number, date):
        conn = sqlite3.connect(DB_PATH)
        cursor = conn.cursor()
        try:
            cursor.execute('''
                INSERT OR IGNORE INTO PairVisits (pair_number, room_id, entered, exited, date)
                VALUES (?, ?, 0, 0, ?)
            ''', (pair_number, self.camera_id, date))
            conn.commit()
        finally:
            conn.close()

    def update_visit_count(self, pair_number, date, entered_delta, exited_delta):
        conn = sqlite3.connect(DB_PATH)
        cursor = conn.cursor()
        try:
            if entered_delta:
                cursor.execute('''
                    UPDATE PairVisits
                    SET entered = entered + 1
                    WHERE pair_number = ? AND room_id = ? AND date = ?
                ''', (pair_number, self.camera_id, date))
            if exited_delta:
                cursor.execute('''
                    UPDATE PairVisits
                    SET exited = exited + 1
                    WHERE pair_number = ? AND room_id = ? AND date = ?
                ''', (pair_number, self.camera_id, date))
            conn.commit()
        finally:
            conn.close()

    def run(self):
        cap = cv2.VideoCapture(self.url)
        if not cap.isOpened():
            print(f"[{self.camera_id}] ❌ Cannot open stream: {self.url}")
            return

        print(f"[{self.camera_id}] ✅ Camera started (visual={self.visual})")

        ret, frame = cap.read()
        if not ret:
            print(f"[{self.camera_id}] ❌ First frame not received.")
            cap.release()
            return

        H, W = frame.shape[:2]
        line_y = int(H * self.line_y_ratio)
        fps = cap.get(cv2.CAP_PROP_FPS) or 30
        delay = 1.0 / fps

        threading.Thread(target=self.detection_loop, daemon=True).start()

        try:
            while True:
                ret, frame = cap.read()
                if not ret:
                    time.sleep(1)
                    continue

                now = datetime.datetime.now()
                today = now.date()
                pair = get_current_pair(now)

                if pair and (pair != self.current_pair or today != self.current_date):
                    self.current_pair = pair
                    self.current_date = today
                    self.ensure_pair_visit_exists(pair, str(today))
                    print(f"[{self.camera_id}] 🔄 Now tracking pair {pair} on {today}")

                with self.detection_lock:
                    self.latest_frame = frame.copy()

                centroids = [(x + w // 2, y + h // 2) for (x, y, w, h) in self.latest_boxes]
                objects = self.tracker.update(centroids)

                for obj_id, (cx, cy) in objects.items():
                    prev = self.cross_state.get(obj_id)
                    curr = 'above' if cy < line_y else 'below'

                    if prev is not None and curr != prev:
                        if pair:
                            if prev == 'above' and curr == 'below':
                                self.entry_count += 1
                                self.update_visit_count(pair, str(today), 1, 0)
                                print(f"[{self.camera_id}] ➕ Entry")
                            elif prev == 'below' and curr == 'above':
                                self.exit_count += 1
                                self.update_visit_count(pair, str(today), 0, 1)
                                print(f"[{self.camera_id}] ➖ Exit")

                    self.cross_state[obj_id] = curr

                if self.visual:
                    for (x, y, w, h) in self.latest_boxes:
                        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
                    cv2.line(frame, (0, line_y), (W, line_y), (0, 0, 255), 2)
                    cv2.putText(frame, f"In: {self.entry_count}", (10, 30),
                                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2)
                    cv2.putText(frame, f"Out: {self.exit_count}", (10, 70),
                                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2)
                    cv2.imshow(f"Camera {self.camera_id}", frame)
                    if cv2.waitKey(1) & 0xFF == ord('q'):
                        break

                time.sleep(delay)

        finally:
            self.running = False
            cap.release()
            if self.visual:
                cv2.destroyAllWindows()
            print(f"[{self.camera_id}] Worker stopped")
