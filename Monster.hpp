#ifndef MONSTER_H
#define MONSTER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>
#include "Player.hpp"
#include <QTimer>
#include <QPixmap>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QLineF>
#include <QSoundEffect>
#include <QElapsedTimer>
#include "HealthBar.hpp"
#include "MyScene.hpp"
#include "bullet.hpp"

class Player;
class MyScene;

class Monster : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Monster(Player* myPlayer, MyScene* mainScene, QObject* parent = nullptr);
    virtual ~Monster();

    int getSpeed() const;
    int getValueScore() const;
    void setValueScore(int s);
    int getHP() const;
    int getDamage() const;
    int getAttackCooldown() const;
    int getSpriteSize() const;

    void setSpeed(int s);
    void setHP(int h);
    void setDamage(int d);
    void setAttackCooldown(int c);
    void setSpriteSize(int size);

    virtual void move();
    virtual void attack();
    void pause();
    void resume();

protected:
    Player* player;
    MyScene* mainScene;
    QTimer* timer;

    int HP;
    int speed;
    int damage;
    int attackCooldown;
    int valueScore = 0;
    int spriteSize = 64;

    QElapsedTimer lastAttackTime;

    QVector<QSoundEffect*> hitSounds;
    int currentHitSoundIndex = 0;

    QTimer* animationTimer = nullptr;
    int currentFrameIndex = 0;
    bool isMoving = false;
    QPixmap* idleSheet;
    QPixmap* moveSheet;

    QVector<QPixmap*> animationUpIdle;
    QVector<QPixmap*> animationDownIdle;
    QVector<QPixmap*> animationLeftIdle;
    QVector<QPixmap*> animationRightIdle;

    QVector<QPixmap*> animationUpMove;
    QVector<QPixmap*> animationDownMove;
    QVector<QPixmap*> animationLeftMove;
    QVector<QPixmap*> animationRightMove;

    virtual void loadAnimations();
    virtual void updateAnimationFrame();

    void clearAnimations();
};

class DoctorMonster : public Monster {
    Q_OBJECT
public:
    DoctorMonster(Player* myPlayer, MyScene* ms, QObject* parent = nullptr);
};

class GhostMonster : public Monster {
    Q_OBJECT
public:
    GhostMonster(Player* myPlayer, MyScene* ms, QObject* parent = nullptr);
    void attack() override;
    void slow();
    QTimer* getSlowTimer() const;
    public slots :
    void resetSpeed();
private:
    QTimer* slowTimer;
};

class BirdMonster : public Monster {
    Q_OBJECT
public:
    BirdMonster(Player* myPlayer, MyScene* ms, QObject* parent = nullptr);
};

#endif // MONSTER_H
