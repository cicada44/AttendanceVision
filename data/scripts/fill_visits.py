import sqlite3
import random
from datetime import date, timedelta

DB_PATH = "../attendance.db"  # укажи путь к своему .db

rooms = [1, 2, 3, 4]  # ID аудиторий
pairs = [1, 2, 3, 4, 5, 6]
start_date = date(2025, 4, 1)
end_date = date(2025, 5, 1)

conn = sqlite3.connect(DB_PATH)
cur = conn.cursor()

for single_date in (start_date + timedelta(n) for n in range((end_date - start_date).days)):
    # пропускаем выходные
    if single_date.weekday() >= 5:
        continue

    for room_id in rooms:
        for pair_number in pairs:
            entered = random.randint(5, 30)
            exited = random.randint(0, entered)  # кто-то мог остаться

            cur.execute("""
                INSERT OR IGNORE INTO PairVisits (pair_number, room_id, entered, exited, date)
                VALUES (?, ?, ?, ?, ?)
            """, (pair_number, room_id, entered, exited, single_date.isoformat()))

conn.commit()
conn.close()
print("Fake data inserted.")
