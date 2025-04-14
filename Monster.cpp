#include "Monster.hpp"



Monster::Monster(Player* myPlayer, QObject* parent)
    : QObject(parent), speed(2), HP(1), player(myPlayer)
{
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
    if (!player) {
        return;
    }
    QPointF monsterPos = this->pos(); //position du monstre
    QPointF playerPos = player->pos(); //position du joueur

    QLineF direction(monsterPos, playerPos);

    double angle = direction.angle(); // on récupère l'angle du monstre par rapport au joueur
    double dx = speed * qCos(qDegreesToRadians(-angle));
    double dy = speed * qSin(qDegreesToRadians(-angle));

    this->moveBy(dx, dy);
}


BigMonster::BigMonster(Player* myPlayer, QObject* parent)
    : Monster(myPlayer, parent)
{
    sprite = QPixmap(":/assets/BigMonster.png");
    if (sprite.isNull()) {
        qWarning("Erreur : le sprite est introuvable !");
        return;
    }
    setPixmap(sprite);
    setSpeed(5);
    setHP(40);
}


SmallMonster::SmallMonster(Player* myPlayer, QObject* parent)
    : Monster(myPlayer, parent)
{
    sprite = QPixmap(":/assets/SmallMonster.png");
    if (sprite.isNull()) {
        qWarning("Erreur : le sprite est introuvable !");
        return;
    }
    setPixmap(sprite);
    setSpeed(8);
    setHP(20);
}