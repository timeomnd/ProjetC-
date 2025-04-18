#include "Player.hpp"
#include <QGraphicsScene>
#include <QDebug>
#include"bullet.hpp"
#include <cmath>

void Player::shoot(const QPointF& targetPos) {
    QPointF direction = targetPos - pos();
    qreal length = std::hypot(direction.x(), direction.y());
    if (length == 0) return;
    
    // Normaliser et ajuster la vitesse
    const qreal projectileSpeed = 15.0;
    QPointF velocity = direction * (projectileSpeed / length);
    
    Projectile* projectile = new Projectile(velocity);
    QPointF playerCenter = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
    projectile->setPos(playerCenter); // Set to center
    scene()->addItem(projectile);
    setFocus();
}

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
}



void Player::keyPressEvent(QKeyEvent* event) {
    pressedKeys.insert(event->key());
    
    dx = 0;
    dy = 0;
    
    if (pressedKeys.contains(Qt::Key_Q)) dx -= speed;
    if (pressedKeys.contains(Qt::Key_D)) dx += speed;
    if (pressedKeys.contains(Qt::Key_Z)) dy -= speed;
    if (pressedKeys.contains(Qt::Key_S)) dy += speed;
    
    // Normalisation
    if (dx != 0 && dy != 0) {
        dx *= 0.7071;
        dy *= 0.7071;
    }
    
    // Mise à jour du sprite uniquement si mouvement
    if (dx < 0) setPixmap(spriteLeft);
    else if (dx > 0) setPixmap(spriteRight);
    else if (dy < 0) setPixmap(spriteUp);
    else if (dy > 0) setPixmap(spriteDown);
}

void Player::keyReleaseEvent(QKeyEvent* event) {
    pressedKeys.remove(event->key());
    
    dx = 0;
    dy = 0;
    
    if (pressedKeys.contains(Qt::Key_Q)) dx -= speed;
    if (pressedKeys.contains(Qt::Key_D)) dx += speed;
    if (pressedKeys.contains(Qt::Key_Z)) dy -= speed;
    if (pressedKeys.contains(Qt::Key_S)) dy += speed;
    
    // Normalisation
    if (dx != 0 && dy != 0) {
        dx *= 0.7071;
        dy *= 0.7071;
    }
    
    // Mise à jour du sprite SEULEMENT s'il y a mouvement
    if (dx < 0) setPixmap(spriteLeft);
    else if (dx > 0) setPixmap(spriteRight);
    else if (dy < 0) setPixmap(spriteUp);
    else if (dy > 0) setPixmap(spriteDown);
    
    // Pas de 'else' : le sprite ne change pas si tout est relâché
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
    HP = h;
}