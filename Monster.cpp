#include "Monster.hpp"



Monster::Monster(Player* myPlayer, QObject* parent): QObject(parent), HP(1), speed(2), player(myPlayer), attackCooldown(1000){
    hitSound = new QSoundEffect(this);
    hitSound->setSource(QUrl("qrc:/assets/hitMonster.wav")); // de préférence .wav
    hitSound->setVolume(0.5);
    lastAttackTime.start();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
    this->move();   // déplacement du monstre
    this->attack(); // attaque du monstre si collision
});
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
int Monster::getAttackCooldown() const {
    return attackCooldown;
}
int Monster::getSpriteSize() const {
    return spriteSize;
}
void Monster::setSpeed(int s) {
    speed = s;
}

void Monster::setHP(int h) {
    if (h <=0) {
        HP = 0;
        delete this;
    }
    else {
        HP = h;
    }
}
void Monster::setDamage(int d) {
    damage = d;
}
void Monster::setAttackCooldown(int c) {
    attackCooldown = c;
}
void Monster::setSpriteSize(int size) {
    spriteSize = size;
}
void Monster::move() {
    if (!player) {
        return;
    }

    QPointF monsterPos = this->pos();  // position du monstre
    QPointF playerPos = player->pos(); // position du joueur

    qreal dx = playerPos.x() - monsterPos.x();  // différence en x
    qreal dy = playerPos.y() - monsterPos.y();  // différence en y

    // Calcul des distances horizontale et verticale
    qreal absDx = qAbs(dx);
    qreal absDy = qAbs(dy);

    // Choisir la direction la plus proche
    if (absDx > absDy) {
        // Déplacement horizontal (gauche/droite)
        if (dx > 0) {
            // Aller à droite
            setPixmap(spriteRight->scaled(spriteSize, spriteSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            this->moveBy(speed, 0);
        } else {
            // Aller à gauche
            setPixmap(spriteLeft->scaled(spriteSize, spriteSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            this->moveBy(-speed, 0);
        }
    } else {
        // Déplacement vertical (haut/bas)
        if (dy > 0) {
            // Aller vers le bas
            setPixmap(spriteDown->scaled(spriteSize, spriteSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            this->moveBy(0, speed);
        } else {
            // Aller vers le haut
            setPixmap(spriteUp->scaled(spriteSize, spriteSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            this->moveBy(0, -speed);
        }
    }
}


void Monster::attack() {
    if (!player) return;
    if (this->collidesWithItem(player)) {
        if (lastAttackTime.elapsed() >= attackCooldown) {
            int newHP = player->getHP() - damage;
            player->setHP(newHP);
            if (player->getHealthBar()) {
                player->getHealthBar()->updateHP(newHP);
            }
            qDebug() << "Attaque ! HP joueur :" << player->getHP();
            hitSound->stop();
            hitSound->play();
            lastAttackTime.restart(); // reset du cooldown
        }
    }
}

BigMonster::BigMonster(Player* myPlayer, QObject* parent)
    : Monster(myPlayer, parent)
{
    setSpeed(5);
    setHP(40);
    setDamage(25);
    setAttackCooldown(2500);
    setSpriteSize(160);
    spriteUp = new QPixmap(":/assets/BigMonster_rear.png");
    spriteDown = new QPixmap(":/assets/BigMonster_front.png");
    spriteLeft = new QPixmap(":/assets/BigMonster_left.png");
    spriteRight = new QPixmap(":/assets/BigMonster_right.png");
    // Vérification des sprites
    if (spriteUp->isNull() || spriteDown->isNull() || spriteLeft->isNull() || spriteRight->isNull()) {
        qWarning("Erreur : un ou plusieurs sprites sont introuvables !");
    }
    setPixmap(spriteUp->scaled(spriteSize, spriteSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

SmallMonster::SmallMonster(Player* myPlayer, QObject* parent)
    : Monster(myPlayer, parent)
{
    setSpeed(7);
    setHP(20);
    setDamage(5);
    setSpriteSize(80);
    spriteUp = new QPixmap(":/assets/SmallMonster_rear.png");
    spriteDown = new QPixmap(":/assets/SmallMonster_front.png");
    spriteLeft = new QPixmap(":/assets/SmallMonster_left.png");
    spriteRight = new QPixmap(":/assets/SmallMonster_right.png");
    // Vérification des sprites
    if (spriteUp->isNull() || spriteDown->isNull() || spriteLeft->isNull() || spriteRight->isNull()) {
        qWarning("Erreur : un ou plusieurs sprites sont introuvables !");
    }
    setPixmap(spriteUp->scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));

}