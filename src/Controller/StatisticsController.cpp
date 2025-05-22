// src/Controller/StatisticsController.cpp
#include "StatisticsController.h"
#include <QSqlError>
#include <QSqlQuery>
#include "../Model/StatsRepository.h"

StatisticsController::StatisticsController(StatsRepository* repo, QObject* parent) : QObject(parent), m_repo(repo) {}

QVector<QPair<QString, QVector<int>>> StatisticsController::fetchEntrySummaryByDate(const QDate& date) {
    QVector<QPair<QString, QVector<int>>> result;

    QSqlQuery query;
    query.prepare(R"(
        SELECT cameras.name, pair_number, entered
        FROM PairVisits
        JOIN cameras ON PairVisits.room_id = cameras.id
        WHERE date = :date
    )");
    query.bindValue(":date", date.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        qWarning() << "Database error in fetchEntrySummaryByDate:" << query.lastError().text();
        return result;
    }

    QMap<QString, QVector<int>> roomToCounts;

    while (query.next()) {
        QString roomName = query.value(0).toString();  // camera name like "408"
        int pair = query.value(1).toInt();
        int entered = query.value(2).toInt();

        if (!roomToCounts.contains(roomName)) {
            roomToCounts[roomName] = QVector<int>(6, 0);
        }

        if (pair >= 1 && pair <= 6) {
            roomToCounts[roomName][pair - 1] += entered;
        }
    }

    for (auto it = roomToCounts.begin(); it != roomToCounts.end(); ++it) {
        result.append(qMakePair(it.key(), it.value()));
    }

    return result;
}

QVector<QPair<QDateTime, int>> StatisticsController::fetchEntriesByPair(const QDate& date, int pairNumber) {
    QVector<QPair<QDateTime, int>> results;

    QSqlQuery query;
    query.prepare("SELECT timestamp, entry_count FROM PairVisits WHERE date(timestamp) = :d AND pair_number = :p");
    query.bindValue(":d", date.toString(Qt::ISODate));
    query.bindValue(":p", pairNumber);

    if (query.exec()) {
        while (query.next()) {
            QDateTime ts = query.value(0).toDateTime();
            int count = query.value(1).toInt();
            results.append({ts, count});
        }
    }

    return results;
}

QVector<QPair<QDateTime, int>> StatisticsController::fetchExitsByPair(const QDate& date, int pairNumber) {
    QVector<QPair<QDateTime, int>> results;

    QSqlQuery query;
    query.prepare("SELECT timestamp, exit_count FROM PairVisits WHERE date(timestamp) = :d AND pair_number = :p");
    query.bindValue(":d", date.toString(Qt::ISODate));
    query.bindValue(":p", pairNumber);

    if (query.exec()) {
        while (query.next()) {
            QDateTime ts = query.value(0).toDateTime();
            int count = query.value(1).toInt();
            results.append({ts, count});
        }
    }

    return results;
}
