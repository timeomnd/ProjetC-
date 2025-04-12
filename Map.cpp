#include "Map.hpp"

Map::Map(QGraphicsScene* scene, QObject* parent) : scene(scene), QObject(parent) {

}
void Map::loadFloor(int level) {

}
void Map:: putWall(QGraphicsScene* scene) {
    for (int i = 0; i<SIZEMAP; i++) {
        for (int j = 0; j < SIZEMAP; j++) {
            if (pos_Wall[i][j]==1){
                // Calcul des coordonnées isométriques
                int x_iso = (j - i) * 32 / 2;  // x' = (x - y) * (32 / 2) : x_iso est calculé en prenant la différence entre les indices (j - i), puis en multipliant par la moitié de la taille de la tuile pour obtenir la projection horizontale.
                int y_iso = (i + j) * 32 / 4;  // y' = (x + y) * (32 / 4) : y_iso est calculé en prenant la somme des indices (i + j), puis en multipliant par un quart de la taille de la tuile pour obtenir la projection verticale.
                QGraphicsRectItem* wall = new QGraphicsRectItem(x_iso, y_iso, 32, 32);
                QPen pen;
                pen.setColor(Qt::black);
                pen.setWidth(2);  // Épaisseur du contour noir
                wall->setPen(pen);
                wall->setBrush(QColor("#fefdeb")); // pas besoin d'utiliser un sprites car les murs d'isen sont juste blanc oeuf
                scene->addItem(wall);
            }
        }
    }
}
/*void Map:: putDoor(QGraphicsScene* scene) {
    for (int i = 0; i<SIZEMAP; i++) {
        for (int j = 0; j < SIZEMAP; j++) {
            if (pos_Wall[i][j]==1){

            }
        }
    }
}*/
/*void Map:: putStairs()(QGraphicsScene* scene) {
    for (int i = 0; i<SIZEMAP; i++) {
        for (int j = 0; j < SIZEMAP; j++) {
            if (pos_Wall[i][j]==1){
                //QGraphicsPixmapItem* stair = new QGraphicsPixmapItem(QPixmap("stairs.png"));
            }
        }
    }
}
*/
void Map::putFloor(QGraphicsScene* scene) {
    for (int i = 0; i < SIZEMAP; i++) {
        for (int j = 0; j < SIZEMAP; j++) {
            if (pos_floor[i][j] == 1) {
                // Calcul des coordonnées isométriques
                int x_iso = (j - i) * 32 / 2;
                int y_iso = (i + j) * 32 / 4;

                QGraphicsRectItem* floor = new QGraphicsRectItem(x_iso, y_iso, 32, 32);
                floor->setBrush(QColor("#cc0000"));
                floor->setPen(Qt::NoPen);
                scene->addItem(floor);
            }
        }
    }
}