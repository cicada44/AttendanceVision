#pragma once

#include <QtWidgets/QtWidgets>
#include <unordered_map>
#include "../Controller/CameraController.h"
#include "../CustomWidgets/CameraLabel.h"

class CamerasTab : public QWidget {
    Q_OBJECT
public:
    explicit CamerasTab(CameraController* controller, QWidget* parent = nullptr);

private:
    void setupUI();
    void fillCamerasLayout(QGridLayout* layout);
    void addNewCamera();

    CameraController* controller;
    std::unordered_map<std::string, CameraPreviewLabel*> videoFeedLabels;
};
