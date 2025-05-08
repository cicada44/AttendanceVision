// src/Model/StatsRepository.cpp
#include "StatsRepository.h"
#include <QDateTime>
#include <QDebug>
#include <QtSql>

StatsRepository::StatsRepository(const QString& dbPath) : m_dbPath(dbPath) {}

static QSqlDatabase openDb(const QString& path) {
    auto db = QSqlDatabase::database("AttendanceConnection");
    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase("QSQLITE", "AttendanceConnection");
    }
    db.setDatabaseName(path);
    if (!db.open()) {
        qWarning() << "Cannot open DB" << path << db.lastError();
    }
    return db;
}

QVector<QPair<QDateTime, int>> StatsRepository::getEntries(const QDateTime& from, const QDateTime& to) {
    QSqlQuery q;
    q.prepare(R"(
        SELECT date, SUM(entered) 
        FROM PairVisits
        WHERE date BETWEEN :from AND :to
        GROUP BY date
        ORDER BY date ASC
    )");
    q.bindValue(":from", from.date());
    q.bindValue(":to", to.date());

    QVector<QPair<QDateTime, int>> res;
    if (q.exec()) {
        while (q.next()) {
            res.emplace_back(QDateTime(q.value(0).toDate(), QTime(0, 0)), q.value(1).toInt());
        }
    } else {
        qWarning() << "StatsRepository::getEntries failed:" << q.lastError().text();
    }
    return res;
}

QVector<QPair<QDateTime, int>> StatsRepository::getExits(const QDateTime& from, const QDateTime& to) {
    QSqlQuery q;
    q.prepare(R"(
        SELECT date, SUM(exited)
        FROM PairVisits
        WHERE date BETWEEN :from AND :to
        GROUP BY date
        ORDER BY date ASC
    )");
    q.bindValue(":from", from.date());
    q.bindValue(":to", to.date());

    QVector<QPair<QDateTime, int>> res;
    if (q.exec()) {
        while (q.next()) {
            res.emplace_back(QDateTime(q.value(0).toDate(), QTime(0, 0)), q.value(1).toInt());
        }
    } else {
        qWarning() << "StatsRepository::getExits failed:" << q.lastError().text();
    }
    return res;
}
