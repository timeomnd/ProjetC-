#ifndef MYSCENE_H
#define MYSCENE_H
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QGraphicsView>
#include "Map.hpp"
#include "Player.hpp"
#include "Monster.hpp"
#include <QPointF>
#include "MainWindow.hpp"
class MainWindow;
class Player;
class Monster;

class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QGraphicsView* mainView, MainWindow* mw = nullptr, QObject* parent = nullptr);
    void initMap();
    void initPlayer();
    virtual ~MyScene();
    void spawnMonster();
    void setPlayerInitialized(bool initialized);

    signals:
        void monsterDestroyed(Monster* monster); // Déclaration du signal

private:
    MainWindow* mainWindow;
    Map* map;
    QTimer* healthbarTimer;
    QTimer* spawnTimer;
    Player* player = nullptr;
    bool playerInitialized = false;
    QList<Monster*> activeMonsters;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif //MYSCENE_H