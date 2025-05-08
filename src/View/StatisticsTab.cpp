#include "StatisticsTab.h"
#include "../Controller/StatisticsController.h"

#include <QDateTimeEdit>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

StatisticsTab::StatisticsTab(StatisticsController* controller, QWidget* parent)
    : QWidget(parent), m_controller(controller) {
    setupUI();
    // loadData();
}

void StatisticsTab::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);

    // --- Параметры периода ---
    auto* controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(new QLabel(tr("From:"), this));
    m_fromEdit = new QDateTimeEdit(QDateTime::currentDateTime().addDays(-1), this);
    m_fromEdit->setCalendarPopup(true);
    controlsLayout->addWidget(m_fromEdit);

    controlsLayout->addWidget(new QLabel(tr("To:"), this));
    m_toEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    m_toEdit->setCalendarPopup(true);
    controlsLayout->addWidget(m_toEdit);

    m_refreshBtn = new QPushButton(tr("Refresh"), this);
    controlsLayout->addWidget(m_refreshBtn);

    mainLayout->addLayout(controlsLayout);

    // --- Таблицы для Entries / Exits ---
    // Entries
    m_entriesTable = new QTableWidget(this);
    m_entriesTable->setColumnCount(2);
    m_entriesTable->setHorizontalHeaderLabels({tr("Timestamp"), tr("Count")});
    m_entriesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Exits
    m_exitsTable = new QTableWidget(this);
    m_exitsTable->setColumnCount(2);
    m_exitsTable->setHorizontalHeaderLabels({tr("Timestamp"), tr("Count")});
    m_exitsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Собираем в табах
    auto* tabs = new QTabWidget(this);
    tabs->addTab(m_entriesTable, tr("Entries"));
    tabs->addTab(m_exitsTable, tr("Exits"));
    mainLayout->addWidget(tabs);

    connect(m_refreshBtn, &QPushButton::clicked, this, &StatisticsTab::onRefreshClicked);
}

void StatisticsTab::onRefreshClicked() { loadData(); }

void StatisticsTab::loadData() {
    const QDateTime from = m_fromEdit->dateTime();
    const QDateTime to = m_toEdit->dateTime();

    // Получаем данные из контроллера
    auto entries = m_controller->fetchEntries(from, to);
    auto exits = m_controller->fetchExits(from, to);

    // Заполняем таблицу Entries
    m_entriesTable->setRowCount(entries.size());
    for (int i = 0; i < entries.size(); ++i) {
        const auto& [ts, cnt] = entries.at(i);
        m_entriesTable->setItem(i, 0, new QTableWidgetItem(ts.toString(Qt::ISODate)));
        m_entriesTable->setItem(i, 1, new QTableWidgetItem(QString::number(cnt)));
    }

    // Заполняем таблицу Exits
    m_exitsTable->setRowCount(exits.size());
    for (int i = 0; i < exits.size(); ++i) {
        const auto& [ts, cnt] = exits.at(i);
        m_exitsTable->setItem(i, 0, new QTableWidgetItem(ts.toString(Qt::ISODate)));
        m_exitsTable->setItem(i, 1, new QTableWidgetItem(QString::number(cnt)));
    }
}
