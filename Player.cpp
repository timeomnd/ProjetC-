#include "Player.hpp"
#include <QGraphicsScene>
#include <QPixmap>

Player::Player(QGraphicsItem* parent): QGraphicsPixmapItem(parent), speed(5), dx(0), dy(0) {
    
    QPixmap sprite(":/assets/nils.png");
    if (!sprite.isNull()) {
        setPixmap(sprite);
    } else {
        qWarning("Erreur : le sprite du joueur est introuvable !");
    }
        

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Player::updatePosition);
    movementTimer->start(16); // ~60 FPS
}

void Player::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Left:
            dx = -speed;
            dy = 0;
            break;
        case Qt::Key_Right:
            dx = speed;
            dy = 0;
            break;
        case Qt::Key_Up:
            dy = -speed;
            dx = 0;
            break;
        case Qt::Key_Down:
            dy = speed;
            dx = 0;
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
