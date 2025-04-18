#pragma once

#include <QtWidgets/QtWidgets>

class VTabBar : public QTabBar {
public:
    QSize tabSizeHint(int index) const;

protected:
    void paintEvent(QPaintEvent *event);
};

class VTabWidget : public QTabWidget {
public:
    VTabWidget(QWidget *parent = 0) : QTabWidget(parent) {
        setTabBar(new VTabBar());
        setTabPosition(QTabWidget::West);
    }
};