import cv2
import numpy as np
import time
from tracker import CentroidTracker

class CameraWorker:
    def __init__(self, camera_id, url, line_y_ratio):
        self.camera_id = camera_id
        self.url = url
        self.line_y_ratio = line_y_ratio
        self.entry_count = 0
        self.exit_count = 0
        self.cross_state = {}  # object_id -> 'above' | 'below'
        self.tracker = CentroidTracker(max_disappeared=40, max_distance=75)

        self.hog = cv2.HOGDescriptor()
        self.hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())

    def run(self):
        cap = cv2.VideoCapture(self.url)
        if not cap.isOpened():
            print(f"[{self.camera_id}] ❌ Не удалось открыть поток: {self.url}")
            return

        print(f"[{self.camera_id}] ✅ Камера запущена")

        ret, frame = cap.read()
        if not ret:
            print(f"[{self.camera_id}] ❌ Первый кадр не получен.")
            return

        H, W = frame.shape[:2]
        line_y = int(H * self.line_y_ratio)

        fps = cap.get(cv2.CAP_PROP_FPS) or 30
        delay = 1.0 / fps  # задержка в секундах

        frame_count = 0

        try:
            while True:
                ret, frame = cap.read()
                if not ret:
                    time.sleep(1)
                    continue

                frame_count += 1
                # только каждый 3-й кадр → снижаем нагрузку
                if frame_count % 3 != 0:
                    time.sleep(delay)
                    continue

                # детекция людей
                rects, _ = self.hog.detectMultiScale(
                    frame, winStride=(8,8), padding=(16,16), scale=1.05
                )
                centroids = [(x + w//2, y + h//2) for (x, y, w, h) in rects]

                # трекинг
                objects = self.tracker.update(centroids)

                for obj_id, (cx, cy) in objects.items():
                    prev = self.cross_state.get(obj_id)
                    curr = 'above' if cy < line_y else 'below'

                    if prev is not None and curr != prev:
                        if prev == 'above' and curr == 'below':
                            self.entry_count += 1
                            print(f"[{self.camera_id}] ➕ Вход")
                        elif prev == 'below' and curr == 'above':
                            self.exit_count += 1
                            print(f"[{self.camera_id}] ➖ Выход")

                    self.cross_state[obj_id] = curr

                time.sleep(delay)
        finally:
            cap.release()
