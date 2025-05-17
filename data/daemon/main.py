import threading
import sqlite3
import os
import time
from dataclasses import dataclass
from camera_worker import CameraWorker
import sys
import io
import signal

sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')

DB_PATH = os.path.join(os.path.dirname(__file__), "../attendance.db")

def handle_sigterm(signum, frame):
    print("[Daemon] SIGTERM received, exiting...")
    exit(0)

@dataclass
class Camera:
    id: int
    name: str
    url: str
    line_y_ratio: float

def load_cameras_from_db():
    if not os.path.exists(DB_PATH):
        print("❌ Database not found:", DB_PATH)
        return []

    conn = sqlite3.connect(DB_PATH)
    cursor = conn.cursor()

    try:
        cursor.execute("SELECT id, name, url, line_y_ratio FROM cameras")
        rows = cursor.fetchall()
        cameras = [Camera(*row) for row in rows]
        return cameras
    except Exception as e:
        print("❌ Failed to load cameras:", e)
        return []
    finally:
        conn.close()

def start_camera_worker(camera: Camera):
    worker = CameraWorker(camera.id, camera.url, camera.line_y_ratio)
    worker.run()  # запускаем обработку (бесконечный цикл)

def main():
    print("✅ Daemon started")
    cameras = load_cameras_from_db()
    if not cameras:
        print("Нет доступных камер.")
        return

    print("📷 Загружено камер:", len(cameras))
    threads = []

    for cam in cameras:
        print(f"🟢 Камера {cam.id} | {cam.name} | {cam.url} | line_y_ratio = {cam.line_y_ratio:.2f}")
        t = threading.Thread(target=start_camera_worker, args=(cam,), daemon=True)
        t.start()
        threads.append(t)

    signal.signal(signal.SIGTERM, handle_sigterm)

    # оставаться в живых
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("🛑 Завешрение демона")


if __name__ == "__main__":
    main()
