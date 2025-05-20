#pragma once

#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStatusBar>
#include <QTimer>
#include <QWidget>
#include "../Controller/CameraController.h"
#include "../Controller/DaemonController.h"
#include "../Controller/StatisticsController.h"
#include "../Model/CameraManager.h"
#include "../View/CamerasTab.h"
#include "../View/DaemonTab.h"
#include "../View/StatisticsTab.h"

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void updateTime();
    void checkNetworkStatus();

private:
    QLabel* timeLabel;
    QLabel* networkLabel;
    QTimer* timer;
    QNetworkAccessManager* networkManager;

    CameraManager* cameraManager;
    CameraController* cameraController;
    CamerasTab* camerasTab;
    DaemonController* daemonController;
    DaemonTab* daemonTab;
    StatsRepository* statsRepo;
    StatisticsController* statsController;
    StatisticsTab* statisticsTab;
};
