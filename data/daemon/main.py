import argparse
import threading
import sqlite3
import os
import time
import sys
import io
import signal
from dataclasses import dataclass
from camera_worker import CameraWorker

# Гарантируем, что консоль печатает в UTF-8
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')

DB_PATH = os.path.join(os.path.dirname(__file__), "../attendance.db")

def handle_sigterm(signum, frame):
    print("[Daemon] SIGTERM received, exiting...")
    sys.exit(0)

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
        return [Camera(*row) for row in rows]
    except Exception as e:
        print("❌ Failed to load cameras:", e)
        return []
    finally:
        conn.close()

def start_camera_worker(camera: Camera, visual: bool):
    worker = CameraWorker(camera.id, camera.url, camera.line_y_ratio, visual)
    worker.run()

def parse_args():
    p = argparse.ArgumentParser(description="AttendanceVision daemon")
    p.add_argument(
        "--visual", action="store_true",
        help="Run in visual mode (show windows and draw boxes)"
    )
    return p.parse_args()

def main():
    args = parse_args()
    visual = args.visual

    print("[Daemon] started in", "visual" if visual else "headless", "mode")

    cameras = load_cameras_from_db()
    if not cameras:
        print("Нет доступных камер.")
        return

    print(f"📷 Loaded {len(cameras)} cameras:")
    for cam in cameras:
        print(f"  • {cam.id}: {cam.name} | {cam.url} | line_y_ratio={cam.line_y_ratio:.2f}")

    # ловим SIGTERM
    signal.signal(signal.SIGTERM, handle_sigterm)

    threads = []
    for cam in cameras:
        t = threading.Thread(
            target=start_camera_worker,
            args=(cam, visual),
            daemon=True
        )
        t.start()
        threads.append(t)

    # держим демон в живых
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("[Daemon] interrupted by user")

if __name__ == "__main__":
    main()
