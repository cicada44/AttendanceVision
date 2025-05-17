#include "CameraController.h"
#include "../View/CameraDialog.h"

CameraController::CameraController(QObject* parent) : QObject(parent), model(nullptr) {}

void CameraController::setModel(CameraManager* m) { model = m; }

void CameraController::handleCameraClicked(const std::string& url) {
    if (!model || !model->openStream(url)) {
        qWarning() << "Failed to open stream for" << QString::fromStdString(url);
        return;
    }

    auto cap = model->getCapture(url);
    CameraDialog dialog(url, cap);
    dialog.exec();
}

bool CameraController::addCamera(const QString& url, const QString& room, double ratio) {
    return model && model->addCamera(url, room, ratio);
}

bool CameraController::removeCamera(const QString& url) { return model && model->removeCamera(url); }

QMap<QString, QString> CameraController::getCameras() const {
    return model ? model->getCameras() : QMap<QString, QString>();
}
