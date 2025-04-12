#include "MyScene.hpp"

MyScene::MyScene(QObject* parent) : QGraphicsScene(parent) {
}

MyScene::~MyScene() {
    delete map;
}
