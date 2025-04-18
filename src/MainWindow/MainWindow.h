#pragma once

#include <QTabWidget>
#include <QWidget>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QTabWidget* sidePanel;  // Боковая панель с вкладками
};
