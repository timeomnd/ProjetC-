
#ifndef MAP_H
#define MAP_H
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#define SIZEMAP 100 // Mur, sol , escalier = 16x16px ou 32x32px personnage = 32x32px ou 48x48px

class Map {
    Q_OBJECT
public:
    Map(QGraphicsScene* scene);
    void loadFloor(int level);
private:
    QGraphicsScene* scene;
    int pos_Wall[SIZEMAP][SIZEMAP];
    int pos_Door[SIZEMAP][SIZEMAP];
    int pos_Stairs[SIZEMAP][SIZEMAP]; // tableau des positions en x,y incrémenter à 1 si un escalier est présent
};
#endif //MAP_H
