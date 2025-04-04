#ifndef MYSCENE_H
#define MYSCENE_H
#include <QGraphicsScene>


class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QObject* parent = nullptr);
    virtual ~MyScene();

};


#endif //MYSCENE_H
