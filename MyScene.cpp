#include "MyScene.hpp"

#include <QGraphicsView>

MyScene::MyScene(QGraphicsView* mainView, QObject* parent) : QGraphicsScene(parent) {
    // Ajout du joueur à la scène
    player = new Player();
    this->addItem(player); // Ajoute le joueur à la scène
    player->setPos(mainView->width()/2, mainView->height()/2); // Position initiale
}
MyScene::MyScene(QObject* parent) : QGraphicsScene(parent) {
}
MyScene::~MyScene() {
    delete map;
}
