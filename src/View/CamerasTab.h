#pragma once

#include <QtWidgets/QtWidgets>
#include <unordered_map>
#include "../Controller/CameraController.h"

class CamerasTab : public QWidget {
    Q_OBJECT
public:
    explicit CamerasTab(CameraController* controller, QWidget* parent = nullptr);

private:
    void setupUI();
    void fillCamerasLayout(QGridLayout* layout);
    void addNewCamera();
    void refresh();

    CameraController* controller;
    QGridLayout* gridLayout = nullptr;
};
