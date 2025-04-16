#include "Monster.hpp"



Monster::Monster(Player* myPlayer, QObject* parent)
    : QObject(parent), speed(2), HP(1), player(myPlayer)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Monster::move);
    timer->start(50);//déplacement toute les 50ms
}

int Monster::getSpeed() const {
    return speed;
}

int Monster::getHP() const {
    return HP;
}
int Monster::getDamage() const {
    return damage;
}
void Monster::setSpeed(int s) {
    speed = s;
}

void Monster::setHP(int h) {
    HP = h;
}
void Monster::setDamage(int d) {
    damage = d;
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

void Monster::attack() {
    if (!player) {
        return;
    }
    if (this->collidesWithItem(player)) {
        player->setHP(player->getHP() - damage);
    }
}
BigMonster::BigMonster(Player* myPlayer, QObject* parent)
    : Monster(myPlayer, parent)
{
    QPixmap pixmap (":/assets/BigMonster.png");
    QPixmap sprite = pixmap.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    if (sprite.isNull()) {
        qWarning("Erreur : le sprite est introuvable !");
        return;
    }
    setPixmap(sprite);
    setSpeed(5);
    setHP(40);
    setDamage(25);
}


SmallMonster::SmallMonster(Player* myPlayer, QObject* parent)
    : Monster(myPlayer, parent)
{
    QPixmap pixmap (":/assets/SmallMonster.png");
    QPixmap sprite = pixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    if (sprite.isNull()) {
        qWarning("Erreur : le sprite est introuvable !");
        return;
    }
    setPixmap(sprite);
    setSpeed(7);
    setHP(20);
    setDamage(5);
}