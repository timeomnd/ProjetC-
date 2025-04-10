#include "MyScene.hpp"

MyScene::MyScene(QObject* parent) : QGraphicsScene(parent) {
    map = new Map(this);
    map->loadFloor(0);
}

MyScene::~MyScene() {
    delete map;
}
