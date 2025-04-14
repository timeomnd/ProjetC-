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



class Monster : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Monster(Player* myPlayer, QObject* parent = nullptr);


    int getSpeed() const;
    int getHP() const;


    void setSpeed(int s);
    void setHP(int h);

    public slots:
        virtual void move();

protected:
    Player* player; //pointeur vers le joueur
    QTimer* timer;
    QPixmap sprite;
    int speed;
    int HP;
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
