
#ifndef MAP_H
#define MAP_H
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#define SIZEMAP 100 // Mur, sol , escalier = 16x16px ou 32x32px personnage = 32x32px ou 48x48px


class Map : public QObject {
    Q_OBJECT
public:
    Map(QGraphicsScene* scene, QObject* parent);
    void loadFloor(int level);
    void putWall(QGraphicsScene* scene);
    void putDoor(QGraphicsScene* scene);
    void putStairs(QGraphicsScene* scene);
    void putFloor(QGraphicsScene* scene);
private:
    QGraphicsScene* scene;
    int pos_Wall[SIZEMAP][SIZEMAP];
    int pos_Door[SIZEMAP][SIZEMAP];
    int pos_Stairs[SIZEMAP][SIZEMAP]; // tableau des positions en x,y incrémenter à 1 si un escalier est présent
    int pos_floor[SIZEMAP][SIZEMAP];
};
#endif //MAP_H
