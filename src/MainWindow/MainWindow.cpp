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
        cameraManager->addCamera(url.toStdString());
        cameraManager->openStream(url.toStdString());
    }

    camerasTab = new CamerasTab(cameraController, this);

    auto sideBar = new VTabWidget();
    sideBar->addTab(camerasTab, "Cameras");

    auto layout = new QHBoxLayout(this);
    layout->addWidget(sideBar);
    setLayout(layout);
}
