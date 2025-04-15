#include <QApplication>
#include "MainWindow.hpp"
#include "MyScene.hpp"
#include <QGraphicsView>

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();
    QGraphicsView* view = mainWindow.findChild<QGraphicsView*>("graphicsView"); // Remplace "graphicsView" par le nom réel de ta vue si besoin

    if (view) {
        // Créer la scène avec la vue
        MyScene* scene = new MyScene(view);
        view->setScene(scene);
        view->setRenderHint(QPainter::Antialiasing);
        view->setMouseTracking(true); // IMPORTANT
        scene->setSceneRect(0, 0, view->width(), view->height());
    }

    return application.exec();
}