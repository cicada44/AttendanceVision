#include "CameraLabel.h"

#include <string>

CameraPreviewLabel::CameraPreviewLabel(const std::string& name, QWidget* parent)
    : QLabel(parent), cameraName(name), hovered(false) {
    setMouseTracking(true);
}

void CameraPreviewLabel::enterEvent(QEnterEvent*) {
    hovered = true;
    update();
}

void CameraPreviewLabel::leaveEvent(QEvent*) {
    hovered = false;
    update();
}

void CameraPreviewLabel::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit cameraClicked(cameraName);
    }
}

void CameraPreviewLabel::paintEvent(QPaintEvent* event) {
    QLabel::paintEvent(event);

    if (hovered) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Полупрозрачное затемнение
        painter.fillRect(rect(), QColor(0, 0, 0, 100));

        // Название камеры
        QFont font = painter.font();
        font.setBold(true);
        font.setPointSize(12);
        painter.setFont(font);
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, QString::fromStdString(cameraName));
    }
}
