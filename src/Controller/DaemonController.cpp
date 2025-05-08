#include "DaemonController.h"
#include <QCoreApplication>

DaemonController::DaemonController(QObject* parent) : QObject(parent), m_process(new QProcess(this)) {
    connect(m_process, &QProcess::started, this, &DaemonController::started);
    connect(m_process, QOverload<QProcess::ProcessError>::of(&QProcess::errorOccurred), this,
            [this](QProcess::ProcessError) { emit errorOccurred(m_process->errorString()); });
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &DaemonController::stopped);
}

DaemonController::~DaemonController() { stopDaemon(); }

bool DaemonController::isRunning() const { return m_process->state() == QProcess::Running; }

void DaemonController::startDaemon() {
    if (isRunning()) return;
    // предполагаем, что ai_daemon.py лежит рядом с бинарником
    const QString python = "python3";
    const QString script = QCoreApplication::applicationDirPath() + "/ai_daemon.py";
    m_process->start(python, {script});
}

void DaemonController::stopDaemon() {
    if (!isRunning()) return;
    m_process->terminate();
    if (!m_process->waitForFinished(3000)) {
        m_process->kill();
        m_process->waitForFinished();
    }
}
