#include "DaemonController.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>

DaemonController::DaemonController(QObject* parent) : QObject(parent), m_process(nullptr) {}

void DaemonController::startDaemon() {
    if (m_process && m_process->state() != QProcess::NotRunning) {
        emit errorOccurred("Daemon already running");
        return;
    }

    m_process = new QProcess(this);

    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() { emit stopped(); });
    connect(m_process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError err) {
        emit errorOccurred("Failed to start daemon: " + m_process->errorString());
    });
    connect(m_process, &QProcess::readyReadStandardOutput, this,
            [=]() { qDebug() << "[Daemon stdout]" << m_process->readAllStandardOutput(); });
    connect(m_process, &QProcess::readyReadStandardError, this,
            [=]() { qDebug() << "[Daemon stderr]" << m_process->readAllStandardError(); });

    QString python = "python3";  // или "python", в зависимости от системы
    QString script = QDir(QCoreApplication::applicationDirPath())
                         .filePath("C:\\cicada-main\\sibsutis\\graduate\\AttendanceVision\\data\\daemon\\main.py");

    m_process->start(python, QStringList() << script);

    if (!m_process->waitForStarted(2000)) {
        emit errorOccurred("Failed to start daemon (timeout)");
        return;
    }

    emit started();
}

void DaemonController::stopDaemon() {
    if (!m_process || m_process->state() == QProcess::NotRunning) return;

    m_process->terminate();  // отправляем SIGTERM
    if (!m_process->waitForFinished(3000)) {
        qDebug() << "⚠️ kill";
        m_process->kill();  // если не завершился — принудительно
        m_process->waitForFinished();
    }

    emit stopped();
}

bool DaemonController::isRunning() const { return m_process && m_process->state() != QProcess::NotRunning; }
