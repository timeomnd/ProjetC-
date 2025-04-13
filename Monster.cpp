#include "Monster.hpp"


// =============== MONSTER ===============
Monster::Monster(QObject* parent)
    : QObject(parent), speed(2), HP(1)
{
    setRect(0, 0, 30, 30);      // Taille par défaut
    setBrush(Qt::red);          // Couleur par défaut

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Monster::move);
    timer->start(50);           // Déplacement toutes les 50 ms (pas encore codé)
}

int Monster::getSpeed() const {
    return speed;
}

int Monster::getHP() const {
    return HP;
}

void Monster::setSpeed(int s) {
    speed = s;
}

void Monster::setHP(int h) {
    HP = h;
}

void Monster::move() {

}

// =============== BIG MONSTER ===============
BigMonster::BigMonster(QObject* parent)
    : Monster(parent)
{
    setRect(0, 0, 50, 50);
    setBrush(Qt::darkRed);
    setSpeed(1);
    setHP(10);
}

// =============== SMALL MONSTER ===============
SmallMonster::SmallMonster(QObject* parent)
    : Monster(parent)
{
    setRect(0, 0, 20, 20);
    setBrush(Qt::magenta);
    setSpeed(4);
    setHP(2);
}