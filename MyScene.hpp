#ifndef MYSCENE_HPP
#define MYSCENE_HPP

#include <QGraphicsScene>
#include <QPointF>
#include "Player.hpp"

class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QGraphicsView* mainView, QObject* parent = nullptr);
    MyScene(QObject* parent = nullptr);
    ~MyScene();


private:
    Player* player;
    QGraphicsItem* map;
};

#endif // MYSCENE_HPP
