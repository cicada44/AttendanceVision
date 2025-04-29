#include <QImage>
#include <QPixmap>
#include <QVBoxLayout>

#include <opencv2/opencv.hpp>

#include "../General/General.h"
#include "CamerasTab.h"

namespace {

CameraPreviewLabel* createVideoLabel() {
    auto videoFeedLabel = new CameraPreviewLabel("Loading video feed...");
    videoFeedLabel->setAlignment(Qt::AlignCenter);
    videoFeedLabel->setStyleSheet("background-color: #333; color: white;");
    videoFeedLabel->setFixedSize(320, 240);
    return videoFeedLabel;
}

}  // namespace

CamerasTab::CamerasTab(QWidget* parent) : QWidget(parent), timer(new QTimer(this)) {
    setupUI();
    startCameras();
}

CamerasTab::~CamerasTab() { stopCamera(); }

void CamerasTab::setupUI() {
    auto layout = new QGridLayout(this);
    fillCamerasLayout(layout);
    setLayout(layout);

    connect(timer, &QTimer::timeout, this, &CamerasTab::updateFrame);
}

void CamerasTab::fillCamerasLayout(QGridLayout* layout) {
    for (const auto& url : CamerasUrls) {
        auto label = createVideoLabel();
        videoFeedLabels[url] = label;
        layout->addWidget(label);
    }
}

void CamerasTab::startCameras() {
    for (const auto& url : CamerasUrls) {
        cv::VideoCapture cap(url);
        if (!cap.isOpened()) {
            videoFeedLabels.at(url)->setText("Failed to open camera!");
            connect(videoFeedLabels.at(url), &CameraPreviewLabel::cameraClicked, this, [&](const std::string& name) {
                auto d = new QDialog(this);
                auto l = new QVBoxLayout(d);
                d->setLayout(l);
            });
            return;
        }
        timer->start(30);
    }
}

void CamerasTab::stopCamera() { timer->stop(); }

void CamerasTab::updateFrame() {
    for (const auto& url : CamerasUrls) {
        auto label = videoFeedLabels.at(url);
        cv::VideoCapture cap(url);
        if (!cap.isOpened()) {
            videoFeedLabels.at(url)->setText("No video feed");
            return;
        }

        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            label->setText("No signal");
            return;
        }

        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

        label->setPixmap(QPixmap::fromImage(qimg).scaled(label->size(), Qt::KeepAspectRatio));
    }
}