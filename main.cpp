#include <QApplication>
#include "MainWindow.hpp"
#include "MyScene.hpp"
#include <QGraphicsView>

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    MainWindow mainWindow;
    mainWindow.showFullScreen();
    return application.exec();
}