#include "DaemonController.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QTextStream>

DaemonController::DaemonController(QObject* parent) : QObject(parent), m_pid(0), m_running(false) {
    // При старте контроллера попытаемся восстановить PID из файла
    m_running = readPidFile();
}

QString DaemonController::pidFilePath() const {
    // Файл daemon.pid рядом с exe-шником
    return QDir(QCoreApplication::applicationDirPath()).filePath("daemon.pid");
}

void DaemonController::writePidFile() {
    QFile f(pidFilePath());
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&f);
        out << m_pid;
        f.close();
    }
}

bool DaemonController::readPidFile() {
    QFile f(pidFilePath());
    if (!f.exists()) return false;
    if (!f.open(QIODevice::ReadOnly)) return false;
    QTextStream in(&f);
    qint64 pid = 0;
    in >> pid;
    f.close();
    if (pid > 0) {
        m_pid = pid;
        return true;
    }
    return false;
}

void DaemonController::removePidFile() { QFile::remove(pidFilePath()); }

void DaemonController::startDaemon(bool visualMode) {
    if (m_running) {
        emit errorOccurred("Daemon already running");
        return;
    }

    // Полные пути к python и скрипту
    QString python = "python3";
    QString script = QDir(QCoreApplication::applicationDirPath()).filePath("../data/daemon/main.py");

    QStringList args;
    args << script;
    if (visualMode) {
        args << "--visual";
    }

    // Запуск detached и получение PID
    qint64 pid = 0;
    bool ok = QProcess::startDetached(python, args, /*workingDirectory=*/QString(), &pid);
    if (!ok || pid == 0) {
        emit errorOccurred("Failed to start daemon in detached mode");
        return;
    }

    m_pid = pid;
    m_running = true;
    writePidFile();
    emit started();

    qDebug() << "[DaemonController] Detached daemon started, PID =" << m_pid;
}

void DaemonController::stopDaemon() {
    if (!m_running || m_pid == 0) {
        // ничего останавливать
        return;
    }

#ifdef Q_OS_WIN
    // Windows: taskkill
    QProcess::execute("taskkill", {"/PID", QString::number(m_pid), "/F"});
#else
    // Unix: kill
    QProcess::execute("kill", {"-TERM", QString::number(m_pid)});
#endif

    // Сброс
    m_pid = 0;
    m_running = false;
    removePidFile();
    emit stopped();

    qDebug() << "[DaemonController] Detached daemon stopped";
}

bool DaemonController::isRunning() const { return m_running; }
