#include "DaemonTab.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "../Controller/DaemonController.h"


DaemonTab::DaemonTab(DaemonController* controller, QWidget* parent) : QWidget(parent), m_controller(controller) {
    m_statusLabel = new QLabel(this);
    m_startBtn = new QPushButton(tr("Start Daemon"), this);
    m_stopBtn = new QPushButton(tr("Stop Daemon"), this);

    auto* vlay = new QVBoxLayout(this);
    vlay->addWidget(m_statusLabel);
    vlay->addWidget(m_startBtn);
    vlay->addWidget(m_stopBtn);
    vlay->addStretch();

    connect(m_startBtn, &QPushButton::clicked, this, &DaemonTab::onStartClicked);
    connect(m_stopBtn, &QPushButton::clicked, this, &DaemonTab::onStopClicked);
    connect(m_controller, &DaemonController::started, this, &DaemonTab::updateStatus);
    connect(m_controller, &DaemonController::stopped, this, &DaemonTab::updateStatus);
    connect(m_controller, &DaemonController::errorOccurred, this, &DaemonTab::showError);

    updateStatus();
}

void DaemonTab::onStartClicked() { m_controller->startDaemon(); }

void DaemonTab::onStopClicked() { m_controller->stopDaemon(); }

void DaemonTab::updateStatus() {
    bool running = m_controller->isRunning();
    m_statusLabel->setText(running ? tr("Daemon status: \u25CF Running") : tr("Daemon status: \u25CB Stopped"));
    m_startBtn->setEnabled(!running);
    m_stopBtn->setEnabled(running);
}

void DaemonTab::showError(const QString& msg) { m_statusLabel->setText(tr("Error: %1").arg(msg)); }
