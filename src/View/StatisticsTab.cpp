#include "StatisticsTab.h"
#include "../Controller/StatisticsController.h"

#include <QComboBox>
#include <QDateEdit>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>


StatisticsTab::StatisticsTab(StatisticsController* controller, QWidget* parent)
    : QWidget(parent), m_controller(controller) {
    setupUI();
}

void StatisticsTab::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);

    // --- Фильтры: дата и пара ---
    auto* controlsLayout = new QHBoxLayout;

    controlsLayout->addWidget(new QLabel(tr("Date:"), this));
    m_dateEdit = new QDateEdit(QDate::currentDate(), this);
    m_dateEdit->setCalendarPopup(true);
    controlsLayout->addWidget(m_dateEdit);

    controlsLayout->addWidget(new QLabel(tr("Pair #"), this));
    m_pairNumber = new QComboBox(this);
    for (int i = 1; i <= 6; ++i) {
        m_pairNumber->addItem(QString::number(i));
    }
    controlsLayout->addWidget(m_pairNumber);

    m_refreshBtn = new QPushButton(tr("Refresh"), this);
    controlsLayout->addWidget(m_refreshBtn);

    mainLayout->addLayout(controlsLayout);

    // --- Таблицы для Entries / Exits ---
    m_entriesTable = new QTableWidget(this);
    m_entriesTable->setColumnCount(2);
    m_entriesTable->setHorizontalHeaderLabels({tr("Timestamp"), tr("Count")});
    m_entriesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_exitsTable = new QTableWidget(this);
    m_exitsTable->setColumnCount(2);
    m_exitsTable->setHorizontalHeaderLabels({tr("Timestamp"), tr("Count")});
    m_exitsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto* tabs = new QTabWidget(this);
    tabs->addTab(m_entriesTable, tr("Entries"));
    tabs->addTab(m_exitsTable, tr("Exits"));

    mainLayout->addWidget(tabs);

    connect(m_refreshBtn, &QPushButton::clicked, this, &StatisticsTab::onRefreshClicked);
}

void StatisticsTab::onRefreshClicked() { loadData(); }

void StatisticsTab::loadData() {
    const QDate date = m_dateEdit->date();
    const int pair = m_pairNumber->currentText().toInt();

    auto entries = m_controller->fetchEntriesByPair(date, pair);
    auto exits = m_controller->fetchExitsByPair(date, pair);

    // Entries
    m_entriesTable->setRowCount(entries.size());
    for (int i = 0; i < entries.size(); ++i) {
        const auto& [ts, cnt] = entries.at(i);
        m_entriesTable->setItem(i, 0, new QTableWidgetItem(ts.toString(Qt::ISODate)));
        m_entriesTable->setItem(i, 1, new QTableWidgetItem(QString::number(cnt)));
    }

    // Exits
    m_exitsTable->setRowCount(exits.size());
    for (int i = 0; i < exits.size(); ++i) {
        const auto& [ts, cnt] = exits.at(i);
        m_exitsTable->setItem(i, 0, new QTableWidgetItem(ts.toString(Qt::ISODate)));
        m_exitsTable->setItem(i, 1, new QTableWidgetItem(QString::number(cnt)));
    }
}
