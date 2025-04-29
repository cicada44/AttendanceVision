#include "CameraController.h"
#include "../View/CameraDialog.h"

CameraController::CameraController(QObject* parent) : QObject(parent), model(nullptr) {}

void CameraController::setModel(CameraManager* m) { model = m; }

void CameraController::handleCameraClicked(std::string& url) {
    if (!model || !model->openStream(url)) {
        qWarning() << "Failed to open stream for" << url;
        return;
    }

    CameraDialog dialog(url, model->getCapture(url));
    dialog.exec();
}
