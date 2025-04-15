#include "Bullet.hpp"
#include <QtMath>
#include <QGraphicsScene>

Bullet::Bullet(QPointF startPos, QPointF targetPos, QGraphicsItem* parent)
    : QGraphicsRectItem(parent), speed(10.0) {

    setRect(0, 0, 10, 4); // Taille du rectangle
    setBrush(QColor("#ffff33")); // Couleur jaune

    setPos(startPos);

    // Calcul direction normalisée
    QPointF direction = targetPos - startPos;
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (length == 0) length = 1; // éviter division par 0
    velocity = (direction / length) * speed;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Bullet::move);
    timer->start(16);
}

void Bullet::move() {
    moveBy(velocity.x(), velocity.y());

    // Supprimer si en dehors de la scène
    if (!scene()->sceneRect().contains(pos())) {
        scene()->removeItem(this);
        deleteLater();
    }
}
