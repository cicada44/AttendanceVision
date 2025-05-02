import cv2
import numpy as np
from collections import OrderedDict

class CentroidTracker:
    def __init__(self, max_disappeared=40, max_distance=50):
        self.next_object_id = 0
        self.objects = OrderedDict()
        self.disappeared = OrderedDict()
        self.max_disappeared = max_disappeared
        self.max_distance = max_distance

    def register(self, centroid):
        self.objects[self.next_object_id] = centroid
        self.disappeared[self.next_object_id] = 0
        self.next_object_id += 1

    def deregister(self, object_id):
        del self.objects[object_id]
        del self.disappeared[object_id]

    def update(self, input_centroids):
        if len(input_centroids) == 0:
            for object_id in list(self.disappeared.keys()):
                self.disappeared[object_id] += 1
                if self.disappeared[object_id] > self.max_disappeared:
                    self.deregister(object_id)
            return self.objects

        if len(self.objects) == 0:
            for centroid in input_centroids:
                self.register(centroid)
        else:
            object_ids = list(self.objects.keys())
            object_centroids = list(self.objects.values())
            D = np.linalg.norm(
                np.array(object_centroids)[:, np.newaxis] - np.array(input_centroids), axis=2
            )
            rows = D.min(axis=1).argsort()
            cols = D.argmin(axis=1)[rows]
            used_rows, used_cols = set(), set()
            for row, col in zip(rows, cols):
                if row in used_rows or col in used_cols or D[row, col] > self.max_distance:
                    continue
                obj_id = object_ids[row]
                self.objects[obj_id] = input_centroids[col]
                self.disappeared[obj_id] = 0
                used_rows.add(row)
                used_cols.add(col)
            for row in set(range(len(object_centroids))) - used_rows:
                obj_id = object_ids[row]
                self.disappeared[obj_id] += 1
                if self.disappeared[obj_id] > self.max_disappeared:
                    self.deregister(obj_id)
            for col in set(range(len(input_centroids))) - used_cols:
                self.register(input_centroids[col])

        return self.objects

def main():
    video_path = 'lecture_recording.avi'
    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        print(f"❌ Cannot open video: {video_path}")
        return

    fps = cap.get(cv2.CAP_PROP_FPS) or 30
    delay = int(1000 / (fps / 2))

    hog = cv2.HOGDescriptor()
    hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())

    tracker = CentroidTracker(max_disappeared=40, max_distance=75)
    entry_count, exit_count = 0, 0
    cross_state = {}

    ret, frame = cap.read()
    if not ret:
        print("❌ Empty video or cannot read first frame.")
        return
    H, W = frame.shape[:2]
    line_y = H // 2  # counting line at center

    while ret:
        frame_draw = frame.copy()
        rects, _ = hog.detectMultiScale(frame, winStride=(8,8), padding=(16,16), scale=1.05)
        centroids = []
        for (x, y, w, h) in rects:
            cx, cy = x + w//2, y + h//2
            centroids.append((cx, cy))
            cv2.rectangle(frame_draw, (x, y), (x+w, y+h), (0,255,0), 2)

        objects = tracker.update(centroids)

        for obj_id, centroid in objects.items():
            cx, cy = centroid
            prev = cross_state.get(obj_id)
            curr = 'above' if cy < line_y else 'below'
            if prev is not None:
                if prev == 'above' and curr == 'below':
                    entry_count += 1
                elif prev == 'below' and curr == 'above':
                    exit_count += 1
            cross_state[obj_id] = curr
            cv2.putText(frame_draw, f"ID{obj_id}", (cx-10, cy-10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,0,0), 2)

        cv2.line(frame_draw, (0, line_y), (W, line_y), (0,0,255), 2)
        cv2.putText(frame_draw, f"Entries: {entry_count}", (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,255), 2)
        cv2.putText(frame_draw, f"Exits: {exit_count}", (10, 70),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,255), 2)

        cv2.imshow("Attendance Analysis", frame_draw)
        if cv2.waitKey(delay) & 0xFF == ord('q'):
            break

        ret, frame = cap.read()

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
