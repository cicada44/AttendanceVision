#pragma once

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

private:
    CameraManager* cameraManager;
    CameraController* cameraController;
    CamerasTab* camerasTab;
    DaemonController* daemonController;
    DaemonTab* daemonTab;
    StatsRepository* statsRepo;
    StatisticsController* statsController;
    StatisticsTab* statisticsTab;
};
