#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

class CameraPreviewLabel : public QLabel {
    Q_OBJECT

public:
    explicit CameraPreviewLabel(const std::string& name, QWidget* parent = nullptr);

    ~CameraPreviewLabel() {};

signals:
    void cameraClicked(std::string& name);

protected:
    void enterEvent(QEnterEvent*) override;

    void leaveEvent(QEvent*) override;

    void mousePressEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent* event) override;

private:
    std::string cameraName;
    bool hovered;
};
