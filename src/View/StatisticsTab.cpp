#include "StatisticsTab.h"
#include "../Controller/StatisticsController.h"

#include <QDateEdit>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

StatisticsTab::StatisticsTab(StatisticsController* controller, QWidget* parent)
    : QWidget(parent), m_controller(controller) {
    setupUI();
}

void StatisticsTab::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);

    // --- Панель выбора даты ---
    auto* controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(new QLabel(tr("Date:"), this));

    m_dateEdit = new QDateEdit(QDate::currentDate(), this);
    m_dateEdit->setCalendarPopup(true);
    controlsLayout->addWidget(m_dateEdit);

    m_refreshBtn = new QPushButton(tr("Refresh"), this);
    controlsLayout->addWidget(m_refreshBtn);

    mainLayout->addLayout(controlsLayout);

    // --- Таблица: Room | Pair 1 | Pair 2 | ... | Pair 6 ---
    m_entriesTable = new QTableWidget(this);
    m_entriesTable->setColumnCount(7);
    QStringList headers = {"Room", "Pair 1", "Pair 2", "Pair 3", "Pair 4", "Pair 5", "Pair 6"};
    m_entriesTable->setHorizontalHeaderLabels(headers);
    m_entriesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_entriesTable->verticalHeader()->hide();
    mainLayout->addWidget(m_entriesTable);

    connect(m_refreshBtn, &QPushButton::clicked, this, &StatisticsTab::onRefreshClicked);
}

void StatisticsTab::onRefreshClicked() {
    const QDate date = m_dateEdit->date();
    auto summary = m_controller->fetchEntrySummaryByDate(date);  // QVector<QPair<QString, QVector<int>>>

    m_entriesTable->setRowCount(summary.size());

    for (int row = 0; row < summary.size(); ++row) {
        const QString& room = summary[row].first;
        const QVector<int>& counts = summary[row].second;

        m_entriesTable->setItem(row, 0, new QTableWidgetItem(room));
        for (int i = 0; i < 6; ++i) {
            m_entriesTable->setItem(row, i + 1, new QTableWidgetItem(QString::number(counts[i])));
        }
    }
}
