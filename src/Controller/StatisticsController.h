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
    QVector<QPair<QDateTime, int>> fetchEntries(const QDateTime& from, const QDateTime& to);
    QVector<QPair<QDateTime, int>> fetchExits(const QDateTime& from, const QDateTime& to);

private:
    StatsRepository* m_repo;
};
