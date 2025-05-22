#pragma once

#include <QtWidgets/QtWidgets>

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
    QCheckBox* m_visualModeCheckbox;

private slots:
    void onStartClicked();
    void onStopClicked();
    void updateStatus();
    void showError(const QString& msg);
};
