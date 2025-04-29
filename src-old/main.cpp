#include <QApplication>
#include <QMainWindow>
#include <QtWidgets/QtWidgets>

#include "MainWindow/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setStyle(QStyleFactory::create("Fusion"));

    QFile styleFile("styles/styles.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        std::string styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
        styleFile.close();
    }

    QMainWindow mainWindow;
    auto centralWidget = new QWidget(&mainWindow);
    mainWindow.setCentralWidget(centralWidget);

    auto mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(new MainWindow());

    mainWindow.setWindowTitle("Attendance");
    mainWindow.resize(1000, 600);
    mainWindow.show();

    return app.exec();
}