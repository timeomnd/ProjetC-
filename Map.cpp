#include "Map.hpp"

Map::Map(QGraphicsScene* scene, QObject* parent) : scene(scene), QObject(parent) {

}
void Map::loadFloor(int level) {

}
void Map:: putWall(QGraphicsScene* scene) {
    for (int i = 0; i<SIZEMAP; i++) {
        for (int j = 0; j < SIZEMAP; j++) {
            if (pos_Wall[i][j]==1){
                QGraphicsRectItem* wall = new QGraphicsRectItem(j, i, 32, 32);
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

                QGraphicsRectItem* floor = new QGraphicsRectItem(j, i, 32, 32);
                floor->setBrush(QColor("#cc0000"));
                floor->setPen(Qt::NoPen);
                scene->addItem(floor);
            }
        }
    }
}