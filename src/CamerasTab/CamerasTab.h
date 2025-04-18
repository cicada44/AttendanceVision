#pragma once

#include <QTimer>
#include <QtWidgets/QtWidgets>
#include <unordered_map>

#include "../CustomWidgets/CameraLabel.h"

class CamerasTab : public QWidget {
    Q_OBJECT

public:
    explicit CamerasTab(QWidget* parent = nullptr);
    ~CamerasTab();

private slots:
    void updateFrame();

private:
    void setupUI();
    void startCameras();
    void stopCamera();

    void fillCamerasLayout(QGridLayout* layout);

    QTimer* timer;
    std::unordered_map<std::string, CameraPreviewLabel*> videoFeedLabels;
    QStackedWidget* stacked;
};