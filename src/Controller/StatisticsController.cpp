// src/Controller/StatisticsController.cpp
#include "StatisticsController.h"
#include "../Model/StatsRepository.h"

StatisticsController::StatisticsController(StatsRepository* repo, QObject* parent) : QObject(parent), m_repo(repo) {}

QVector<QPair<QDateTime, int>> StatisticsController::fetchEntries(const QDateTime& f, const QDateTime& t) {
    return m_repo->getEntries(f, t);
}

QVector<QPair<QDateTime, int>> StatisticsController::fetchExits(const QDateTime& f, const QDateTime& t) {
    return m_repo->getExits(f, t);
}
