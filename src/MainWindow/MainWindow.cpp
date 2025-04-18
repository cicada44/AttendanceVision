#include <QHBoxLayout>

#include "../AttendanceTab/AttendanceTab.h"
#include "../CamerasTab/CamerasTab.h"
#include "../CustomWidgets/VTabWidget.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    sidePanel = new VTabWidget();

    sidePanel->setTabPosition(QTabWidget::West);

    auto camerasTab = new CamerasTab(this);
    auto attendanceTab = new AttendanceTab(this);

    sidePanel->addTab(camerasTab, "Cameras");
    sidePanel->addTab(attendanceTab, "Attendance");

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(sidePanel);

    setLayout(mainLayout);
}