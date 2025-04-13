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



// =============== Classe de base : MONSTER ===============
class Monster : public QObject, public QGraphicsRectItem {
    Q_OBJECT

public:
    Monster(QObject* parent = nullptr);

    // Getters
    int getSpeed() const;
    int getHP() const;

    // Setters
    void setSpeed(int s);
    void setHP(int h);

    public slots:
        virtual void move();

protected:
    QTimer* timer;
    int speed;
    int HP;
};

// =============== BigMonster ====================
class BigMonster : public Monster {
public:
    BigMonster(QObject* parent = nullptr);
};

// =============== SmallMonster ====================
class SmallMonster : public Monster {
public:
    SmallMonster(QObject* parent = nullptr);
};

#endif //MONSTER_H
