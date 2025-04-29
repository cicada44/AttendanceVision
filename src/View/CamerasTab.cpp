#include "CamerasTab.h"
#include <QGridLayout>
#include "../General/General.h"

CamerasTab::CamerasTab(CameraController* controller, QWidget* parent) : QWidget(parent), controller(controller) {
    setupUI();
}

void CamerasTab::setupUI() {
    auto layout = new QGridLayout(this);
    fillCamerasLayout(layout);
    setLayout(layout);
}

void CamerasTab::fillCamerasLayout(QGridLayout* layout) {
    int row = 0, col = 0;
    for (const auto& url : CamerasUrls) {
        auto label = new CameraPreviewLabel(url, this);
        videoFeedLabels[url] = label;
        layout->addWidget(label, row, col);

        connect(label, &CameraPreviewLabel::cameraClicked, this,
                [=, this](std::string& name) { controller->handleCameraClicked(name); });

        col++;
        if (col >= 2) {  // 2 камеры в строке
            col = 0;
            row++;
        }
    }
}
