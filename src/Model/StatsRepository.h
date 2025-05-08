#pragma once

#include <QDateTime>
#include <QPair>
#include <QString>
#include <QVector>

class StatsRepository {
public:
    explicit StatsRepository(const QString& dbPath);

    // возвращает список <timestamp, count> за заданный интервал
    QVector<QPair<QDateTime, int>> getEntries(const QDateTime& from, const QDateTime& to);
    QVector<QPair<QDateTime, int>> getExits(const QDateTime& from, const QDateTime& to);

private:
    QString m_dbPath;
};
