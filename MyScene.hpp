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

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    
private:
    Player* player;
    QPointF lastMousePos;
    QGraphicsItem* map;
};

#endif // MYSCENE_HPP
