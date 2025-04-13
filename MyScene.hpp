#ifndef MYSCENE_H
#define MYSCENE_H
#include <QGraphicsScene>
#include "Map.hpp"
#include "Player.hpp"
class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QObject* parent = nullptr);
    MyScene(QGraphicsView* mainView, QObject* parent = nullptr);
    virtual ~MyScene();
private:
    Map* map;
    Player* player;
};


#endif //MYSCENE_H
