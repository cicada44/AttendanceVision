#pragma once

#include <QObject>
#include "../Model/CameraManager.h"

class CameraController : public QObject {
    Q_OBJECT
public:
    explicit CameraController(QObject* parent = nullptr);

    void setModel(CameraManager* model);
    void handleCameraClicked(std::string& url);

private:
    CameraManager* model;
};
