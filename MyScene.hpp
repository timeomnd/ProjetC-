#ifndef MYSCENE_H
#define MYSCENE_H
#include <QGraphicsScene>
#include "Map.hpp"

class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QObject* parent = nullptr);
    virtual ~MyScene();
private:
    Map* map;
};


#endif //MYSCENE_H
