#include "CameraController.h"
#include "../View/CameraDialog.h"

CameraController::CameraController(QObject* parent) : QObject(parent), model(nullptr) {}

void CameraController::setModel(CameraManager* m) { model = m; }

void CameraController::handleCameraClicked(const std::string& url) {
    if (!model || !model->openStream(url)) {
        qWarning() << "Failed to open stream for" << url;
        return;
    }

    auto cap = model->getCapture(url);
    CameraDialog dialog(url, cap);
    dialog.exec();
}
