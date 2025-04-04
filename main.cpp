#include <QApplication>
#include "MainWindow.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return application.exec();
}