#pragma once

#include <QWidget>

class QPushButton;
class QLabel;
class DaemonController;

class DaemonTab : public QWidget {
    Q_OBJECT
public:
    explicit DaemonTab(DaemonController* controller, QWidget* parent = nullptr);

private:
    DaemonController* m_controller;
    QPushButton* m_startBtn;
    QPushButton* m_stopBtn;
    QLabel* m_statusLabel;

private slots:
    void onStartClicked();
    void onStopClicked();
    void updateStatus();
    void showError(const QString& msg);
};
