#include "MainWindow.h"
#include <QVBoxLayout>
#include "General/General.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    cameraManager = new CameraManager();
    cameraController = new CameraController(this);
    cameraController->setModel(cameraManager);

    // Предзаполняем камеры из General.h
    for (const auto& url : CamerasUrls) {
        cameraManager->addCamera(url);
    }

    camerasTab = new CamerasTab(cameraController, this);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(camerasTab);
    setLayout(layout);
}
