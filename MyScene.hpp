#ifndef MYSCENE_H
#define MYSCENE_H
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QGraphicsView>
#include "Map.hpp"
#include "Player.hpp"
#include "Monster.hpp"
#include <QPointF>
class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QObject* parent = nullptr);
    MyScene(QGraphicsView* mainView, QObject* parent = nullptr);
    virtual ~MyScene();
private:
    Map* map;
    Player* player;
    QTimer* healthbarTimer;
    QTimer* spawnTimer;
    private slots:
    void spawnMonster();
};


#endif //MYSCENE_H