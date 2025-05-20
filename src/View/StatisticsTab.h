#pragma once

#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QWidget>

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
    QDateEdit* m_dateEdit;
    QComboBox* m_pairNumber;
    QPushButton* m_refreshBtn;
    QTableWidget* m_entriesTable;
    QTableWidget* m_exitsTable;
};
