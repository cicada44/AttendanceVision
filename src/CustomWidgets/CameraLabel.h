#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

class CameraPreviewLabel : public QLabel {
    Q_OBJECT

public:
    explicit CameraPreviewLabel(const QString& name, QWidget* parent = nullptr);

    ~CameraPreviewLabel() {};

signals:
    void cameraClicked(const QString& name);

protected:
    void enterEvent(QEnterEvent*) override;

    void leaveEvent(QEvent*) override;

    void mousePressEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent* event) override;

private:
    QString cameraName;
    bool hovered;
};
