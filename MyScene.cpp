#include "MyScene.hpp"
#include <QGraphicsSceneMouseEvent>
#include "Bullet.hpp"
#include "Player.hpp"
#include <QGraphicsView>

MyScene::MyScene(QGraphicsView* mainView, QObject* parent) 
    : QGraphicsScene(parent), map(nullptr) 
{
    // Ajout du joueur à la scène
    player = new Player();
    this->addItem(player);
    player->setPos(mainView->width() / 2, mainView->height() / 2); // Position initiale
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    lastMousePos = event->scenePos();
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton && player) {
        QPointF playerCenter = player->sceneBoundingRect().center();
        Bullet* bullet = new Bullet(playerCenter, lastMousePos);
        addItem(bullet);
    }
}

// Constructeur alternatif (inutile si non utilisé ailleurs)
MyScene::MyScene(QObject* parent) 
    : QGraphicsScene(parent), map(nullptr) 
{
}

MyScene::~MyScene() {
    delete map;
}