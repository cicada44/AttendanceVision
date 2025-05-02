import cv2
import time

def record_stream(url: str, output_file: str, duration_sec: int = 180):
    """
    Records video from an MJPG stream for a specified duration.

    :param url: URL of the MJPG stream.
    :param output_file: Path to save the recorded video (e.g., 'output.avi').
    :param duration_sec: Recording duration in seconds (default: 180 seconds).
    """
    cap = cv2.VideoCapture(url)
    if not cap.isOpened():
        print(f"❌ Failed to open stream: {url}")
        return

    # Get stream properties
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = cap.get(cv2.CAP_PROP_FPS) or 20.0  # default to 20 if FPS is unavailable

    # Setup video writer (XVID codec for .avi output)
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    writer = cv2.VideoWriter(output_file, fourcc, fps, (width, height))

    start_time = time.time()
    print(f"▶️ Recording started: saving to '{output_file}' for {duration_sec} seconds...")

    while True:
        ret, frame = cap.read()
        if not ret:
            print("⚠️ Frame grab failed; stopping early.")
            break

        writer.write(frame)

        # Stop after duration
        if time.time() - start_time > duration_sec:
            break

    cap.release()
    writer.release()
    print(f"✅ Recording finished. Video saved to '{output_file}'")

if __name__ == "__main__":
    stream_url = "http://admin:0gfhjkm@192.168.6.61/video2.mjpg"
    record_stream(stream_url, "lecture_recording.avi", duration_sec=180)
