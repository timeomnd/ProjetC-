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
#include "Score.hpp"
class MainWindow;
class Player;
class Monster;

class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QGraphicsView* mainView, MainWindow* mw = nullptr, QObject* parent = nullptr);
    void initMap();
    void initPlayer();
    Player* getPlayer();
    virtual ~MyScene();
    void spawnMonster();
    void setPlayerInitialized(bool initialized);
    QTimer* getHealthbarTimer() const;
    QTimer* getSpawnTimer() const;
    QTimer* getScoreTimer() const;
    QList<Monster*>& getActiveMonsters();
    ScoreManager* getScoreManager() const;
    void setHealthbarTimer(QTimer* timer);
    void setSpawnTimer(QTimer* timer);
    void initScoreManager();
    void destroyMonster(Monster* monster);


    signals:
        void monsterDestroyed(Monster* monster); // Déclaration du signal

private:
    MainWindow* mainWindow;
    Map* map;
    ScoreManager* scoreManager;
    QTimer* healthbarTimer;
    QTimer* spawnTimer;
    QTimer* scoreTimer;
    Player* player = nullptr;
    bool playerInitialized = false;
    QList<Monster*> activeMonsters;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif //MYSCENE_H