#pragma once

#include <QDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPainter>
#include <QTcpSocket>
#include <QTimer>
#include <opencv2/opencv.hpp>


class CameraDialog : public QDialog {
    Q_OBJECT
public:
    CameraDialog(std::string url, cv::VideoCapture* cap, QWidget* parent = nullptr);

private:
    QLabel* videoLabel;
    QTimer* timer;
    cv::VideoCapture* capture;
    QTcpSocket* socket = nullptr;
    QTimer* boxPollTimer = nullptr;
    QVector<QRect> personRects;
    bool drawBoxes = false;
};
