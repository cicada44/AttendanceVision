#pragma once

#include <QObject>

class DaemonController : public QObject {
    Q_OBJECT

public:
    explicit DaemonController(QObject* parent = nullptr);

    /// Запускает демон (detached). Если visualMode==true, добавляет --visual.
    void startDaemon(bool visualMode = false);
    /// Останавливает демон по сохранённому PID.
    void stopDaemon();
    /// Возвращает true, если демон по PID из файла запущен.
    bool isRunning() const;

signals:
    void started();
    void stopped();
    void errorOccurred(const QString& message);

private:
    qint64 m_pid;
    bool m_running;

    QString pidFilePath() const;
    void writePidFile();
    void removePidFile();
    bool readPidFile();
};
