// src/Controller/StatisticsController.h
#pragma once
#include <QDateTime>
#include <QObject>
#include <QPair>
#include <QVector>

#include "../Model/StatsRepository.h"

class StatisticsController : public QObject {
    Q_OBJECT
public:
    explicit StatisticsController(StatsRepository* repo, QObject* parent = nullptr);

    // вызывается из GUI
    QVector<QPair<QDateTime, int>> fetchEntriesByPair(const QDate& date, int pairNumber);
    QVector<QPair<QDateTime, int>> fetchExitsByPair(const QDate& date, int pairNumber);

private:
    StatsRepository* m_repo;
};
