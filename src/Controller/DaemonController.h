#pragma once

#include <QObject>
#include <QProcess>

class DaemonController : public QObject {
    Q_OBJECT
public:
    explicit DaemonController(QObject* parent = nullptr);
    ~DaemonController();

    bool isRunning() const;
public slots:
    void startDaemon();
    void stopDaemon();

signals:
    void started();
    void stopped();
    void errorOccurred(const QString& error);

private:
    QProcess* m_process;
};
