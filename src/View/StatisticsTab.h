#pragma once

#include <QDateTime>
#include <QtWidgets/QtWidgets>

#include "StatisticsController.h"

class StatisticsTab : public QWidget {
    Q_OBJECT
public:
    explicit StatisticsTab(StatisticsController* controller, QWidget* parent = nullptr);

private slots:
    void onRefreshClicked();

private:
    void setupUI();
    void loadData();

    StatisticsController* m_controller;
    QDateTimeEdit* m_fromEdit;
    QDateTimeEdit* m_toEdit;
    QPushButton* m_refreshBtn;
    QTableWidget* m_entriesTable;
    QTableWidget* m_exitsTable;
};
