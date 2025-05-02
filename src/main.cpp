#include <QApplication>
#include "MainWindow/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");

    QFont f = app.font();
    f.setPointSize(16);
    app.setFont(f);

    MainWindow w;
    w.show();

    return app.exec();
}
