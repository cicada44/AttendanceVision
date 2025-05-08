#pragma once

#include <QObject>
#include "../Model/CameraManager.h"

class CameraController : public QObject {
    Q_OBJECT
public:
    explicit CameraController(QObject* parent = nullptr);

    void setModel(CameraManager* model);
    void handleCameraClicked(const std::string& url);

    bool addCamera(const QString& url, const QString& room);
    bool removeCamera(const QString& url);

    QMap<QString, QString> getCameras() const;

private:
    CameraManager* model;
};
