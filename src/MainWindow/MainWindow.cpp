#include "MainWindow.h"
#include <QVBoxLayout>
#include "General/General.h"

#include "CustomWidgets/VTabWidget.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    cameraManager = new CameraManager();
    cameraController = new CameraController(this);
    cameraController->setModel(cameraManager);

    // Предзаполняем камеры из General.h
    for (const auto& [url, name] : CameraRoomMap) {
        cameraManager->addCamera(url, name);
        cameraManager->openStream(url.toStdString());
    }

    camerasTab = new CamerasTab(cameraController, this);

    daemonController = new DaemonController(this);
    daemonTab = new DaemonTab(daemonController, this);

    const QString dbPath = QCoreApplication::applicationDirPath() + "/data/attendance.db";
    statsRepo = new StatsRepository(dbPath);
    statsController = new StatisticsController(statsRepo, this);
    statisticsTab = new StatisticsTab(statsController, this);

    auto sideBar = new VTabWidget();
    sideBar->addTab(camerasTab, "Cameras");
    sideBar->addTab(daemonTab, "Daemon");
    sideBar->addTab(statisticsTab, tr("Statistics"));

    auto layout = new QHBoxLayout(this);
    layout->addWidget(sideBar);
    setLayout(layout);
}
