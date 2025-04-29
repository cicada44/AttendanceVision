#include "CameraDialog.h"
#include <QImage>
#include <QPushButton>
#include <QVBoxLayout>

CameraDialog::CameraDialog(std::string url, cv::VideoCapture* cap, QWidget* parent) : QDialog(parent), capture(cap) {
    setWindowTitle(QString::fromStdString(url));
    resize(800, 600);

    auto layout = new QVBoxLayout(this);
    videoLabel = new QLabel(this);
    videoLabel->setAlignment(Qt::AlignCenter);
    videoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto filterButton = new QPushButton("Enable Filter (stub)", this);

    layout->addWidget(videoLabel);
    layout->addWidget(filterButton);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        if (capture && capture->isOpened()) {
            cv::Mat frame;
            (*capture) >> frame;
            if (!frame.empty()) {
                cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
                QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
                videoLabel->setPixmap(QPixmap::fromImage(img).scaled(videoLabel->size(), Qt::KeepAspectRatio));
            }
        }
    });
    timer->start(30);

    connect(filterButton, &QPushButton::clicked, this, [=]() { qDebug() << "Filter button clicked (stub)"; });
}
