#include "global.h"
#include "mainwindow.h"
#include "runguard.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    RunGuard guard("some_random_key");
    if (!guard.tryToRun())
        return 0;
    QApplication a(argc, argv);
    QApplication::setOrganizationName("DP Software");
    QApplication::setApplicationName(APPNAME);
    QApplication::setApplicationVersion(VERSION);

    MainWindow w;
    w.show();
    return a.exec();
}
