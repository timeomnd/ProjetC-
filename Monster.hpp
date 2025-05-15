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

class Player;

class Monster : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Monster(Player* myPlayer,MyScene* mainScene, QObject* parent = nullptr);


    int getSpeed() const;
    int getHP() const;
    int getDamage() const;
    int getAttackCooldown() const;
    int getSpriteSize() const;
    int getValueScore() const;
    void setValueScore(int s);
    void setSpeed(int s);
    void setHP(int h);
    void setDamage(int d);
    void setAttackCooldown(int c);
    void setSpriteSize(int size);
    virtual ~Monster();

    public slots:
        virtual void move();
        virtual void attack();
protected:
    int spriteSize;
    int HP;
    MyScene* mainScene;
    Player* player; //pointeur vers le joueur
    QTimer* timer;
    QPixmap* spriteUp;
    QPixmap* spriteDown;
    QPixmap* spriteLeft;
    QPixmap* spriteRight;
    QVector<QSoundEffect*> hitSounds;
    int currentHitSoundIndex = 0;
    QElapsedTimer lastAttackTime;
    int attackCooldown; // en millisecondes
    int speed;
    int damage;
    int valueScore;
};


class BigMonster : public Monster {
public:
    BigMonster(Player* myPlayer, MyScene* ms, QObject* parent = nullptr);
};


class SmallMonster : public Monster {
public:
    SmallMonster(Player* myPlayer, MyScene* ms, QObject* parent = nullptr);
};

#endif //MONSTER_H
