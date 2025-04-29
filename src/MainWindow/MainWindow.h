#pragma once

#include <QWidget>
#include "../Controller/CameraController.h"
#include "../Model/CameraManager.h"
#include "../View/CamerasTab.h"

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    CameraManager* cameraManager;
    CameraController* cameraController;
    CamerasTab* camerasTab;
};
