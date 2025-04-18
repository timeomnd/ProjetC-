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


class Monster : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Monster(Player* myPlayer, QObject* parent = nullptr);


    int getSpeed() const;
    int getHP() const;
    int getDamage() const;
    int getAttackCooldown() const;
    int getSpriteSize() const;


    void setSpeed(int s);
    void setHP(int h);
    void setDamage(int d);
    void setAttackCooldown(int c);
    void setSpriteSize(int size);

    public slots:
        virtual void move();
        virtual void attack();

protected:
    int spriteSize;
    int HP;
    Player* player; //pointeur vers le joueur
    QTimer* timer;
    QTimer* healthTimer;
    QPixmap* spriteUp;
    QPixmap* spriteDown;
    QPixmap* spriteLeft;
    QPixmap* spriteRight;
    QSoundEffect* hitSound;
    QElapsedTimer lastAttackTime;
    int attackCooldown; // en millisecondes
    int speed;
    int damage;
};


class BigMonster : public Monster {
public:
    BigMonster(Player* myPlayer, QObject* parent = nullptr);
};


class SmallMonster : public Monster {
public:
    SmallMonster(Player* myPlayer, QObject* parent = nullptr);
};

#endif //MONSTER_H
