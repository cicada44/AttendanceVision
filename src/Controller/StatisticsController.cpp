// src/Controller/StatisticsController.cpp
#include "StatisticsController.h"
#include <QSqlQuery>
#include "../Model/StatsRepository.h"

StatisticsController::StatisticsController(StatsRepository* repo, QObject* parent) : QObject(parent), m_repo(repo) {}

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
