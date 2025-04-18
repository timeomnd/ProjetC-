#include "bullet.hpp"
#include <QGraphicsScene>
#include <QtMath>

Projectile::Projectile(const QPointF& velocity, QGraphicsItem* parent): QGraphicsPixmapItem(parent), velocity(velocity){
    
    setPixmap(QPixmap(":/assets/bullet.png"));

    qreal w = pixmap().width();
    qreal h = pixmap().height();
    setOffset(-w/2, -h/2);

    setFlag(QGraphicsItem::ItemIsFocusable, false);

    applyRotation(); // 🔥 APPEL DE LA ROTATION

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Projectile::move);
    moveTimer->start(16);

    lifespanTimer = new QTimer(this);
    connect(lifespanTimer, &QTimer::timeout, this, &Projectile::destroy);
    lifespanTimer->start(1000);
}

void Projectile::applyRotation() {
    // Calcule l'angle en degrés à partir du vecteur de vélocité
    qreal angle = std::atan2(velocity.y(), velocity.x()) * 180 / M_PI;
    setRotation(angle);
}

void Projectile::move() {
    setPos(pos() + velocity);
    // Vérification hors de la scène
    if (!scene() || !scene()->sceneRect().contains(pos())) {
        destroy();
    }
}

void Projectile::destroy() {
    moveTimer->stop();       // Ajoutez ces deux lignes
    lifespanTimer->stop();
    if (scene()) scene()->removeItem(this);
    deleteLater();
}