import sqlite3
import os

db_path = os.path.join(os.path.dirname(__file__), 'attendance.db')

# Удаляем старую версию для тестов (не делай этого в бою)
if os.path.exists(db_path):
    os.remove(db_path)

conn = sqlite3.connect(db_path)
cursor = conn.cursor()

cursor.execute('''
CREATE TABLE cameras (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    url TEXT NOT NULL
)
''')

cursor.execute('''
CREATE TABLE IF NOT EXISTS PairVisits (
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    pair_number INTEGER NOT NULL CHECK(pair_number BETWEEN 1 AND 6), -- Номер пары (1–6)
    room_id     INTEGER NOT NULL,                                    -- Ссылка на аудиторию
    entered     INTEGER NOT NULL DEFAULT 0,                          -- Входов за пару
    exited      INTEGER NOT NULL DEFAULT 0,                          -- Выходов за пару
    date        DATE NOT NULL,                                       -- Дата проведения пары

    FOREIGN KEY (room_id) REFERENCES Rooms(id),
    UNIQUE(date, pair_number, room_id)                               -- Один уникальный отчёт на пару
);
''')

conn.commit()
conn.close()
print("База данных инициализирована.")
