#include <QApplication>
#include "General/General.h"
#include "MainWindow/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    // app.setStyle("Fusion");

    QFont f = app.font();
    f.setPointSize(16);
    app.setFont(f);

    if (!initDatabaseConnection()) return -1;

    MainWindow w;
    w.show();

    return app.exec();
}
