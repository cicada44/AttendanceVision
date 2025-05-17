#pragma once

#include <QObject>
#include <QProcess>

class DaemonController : public QObject {
    Q_OBJECT

public:
    explicit DaemonController(QObject* parent = nullptr);
    void startDaemon();
    void stopDaemon();
    bool isRunning() const;

signals:
    void started();
    void stopped();
    void errorOccurred(const QString& message);

private:
    QProcess* m_process;
};
