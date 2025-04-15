#include "MyScene.hpp"
#include <QGraphicsSceneMouseEvent>
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


// Constructeur alternatif (inutile si non utilisé ailleurs)
MyScene::MyScene(QObject* parent) 
    : QGraphicsScene(parent), map(nullptr) 
{
}

MyScene::~MyScene() {
    delete map;
}