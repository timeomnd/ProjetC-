#include "Player.hpp"
#include <QGraphicsScene>
#include <QDebug>

Player::Player(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), speed(5), dx(0), dy(0) {
    setHP(100);
    // Chargement des sprites
    spriteUp = QPixmap(":/assets/nils_rear.png");
    spriteDown = QPixmap(":/assets/nils_front.png");
    spriteLeft = QPixmap(":/assets/nils_left.png");
    spriteRight = QPixmap(":/assets/nils_right.png");

    // Vérification des sprites
    if (spriteUp.isNull() || spriteDown.isNull() || spriteLeft.isNull() || spriteRight.isNull()) {
        qWarning("Erreur : un ou plusieurs sprites sont introuvables !");
    }

    // Sprite initial (face avant)
    setPixmap(spriteDown);

    // Configuration des événements clavier
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    // Timer pour le mouvement
    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Player::updatePosition);
    movementTimer->start(16); // ~60 FPS

    //Barre de vie
    healthBar = new HealthBar(100);
}

void Player::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Q:
            dx = -speed;
            dy = 0;
            setPixmap(spriteLeft); // Sprite gauche
            break;
        case Qt::Key_D:
            dx = speed;
            dy = 0;
            setPixmap(spriteRight); // Sprite droite
            break;
        case Qt::Key_Z:
            dy = -speed;
            dx = 0;
            setPixmap(spriteUp); // Sprite haut
            break;
        case Qt::Key_S:
            dy = speed;
            dx = 0;
            setPixmap(spriteDown); // Sprite bas
            break;
        default:
            break;
    }
}

void Player::keyReleaseEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Q:
        case Qt::Key_D:
            dx = 0; // Arrêt du mouvement horizontal
            break;
        case Qt::Key_Z:
        case Qt::Key_S:
            dy = 0; // Arrêt du mouvement vertical
            break;
        default:
            break;
    }
}

void Player::updatePosition() {
    moveBy(dx, dy);
}

void Player::setSpeed(int s) {
    speed = s;
}

int Player::getSpeed() const {
    return speed;
}
int Player::getHP() const {
    return HP;
}
void Player::setHP(int h) {
    if (h <=0) {
        HP = 0;
    }
    else {
        HP = h;
    }
}
HealthBar* Player::getHealthBar() const {
    return healthBar;
}
