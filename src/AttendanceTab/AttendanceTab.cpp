#include "AttendanceTab.h"
#include <QLabel>
#include <QVBoxLayout>

AttendanceTab::AttendanceTab(QWidget* parent) : QWidget(parent) {
    // Временное содержимое вкладки
    auto layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Attendance Data Placeholder", this));
    setLayout(layout);
}