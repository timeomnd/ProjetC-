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
    MyScene(QGraphicsView* mainView, QObject* parent = nullptr);
    void initPlayer();
    virtual ~MyScene();
    void spawnMonster();
    void setPlayerInitialized(bool initialized);
private:
    Map* map;
    QTimer* spawnTimer;
    Player* player = nullptr;
    bool playerInitialized = false;
    QList<Monster*> activeMonsters;


    protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};


#endif //MYSCENE_H