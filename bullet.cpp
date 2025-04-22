#include "bullet.hpp"
#include <QGraphicsScene>
#include <QtMath>

Projectile::Projectile(const QPointF& velocity, QGraphicsItem* parent): QGraphicsPixmapItem(parent), velocity(velocity){
    
    setPixmap(QPixmap(":/assets/bullet.png"));
    setDamage(10);
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
    QList<QGraphicsItem*> collisions = collidingItems();
    for (QGraphicsItem* item : collisions) {
        Monster* monster = dynamic_cast<Monster*>(item);
        if (monster) {
            monster->setHP(monster->getHP() - getDamage());

            if (monster->getHP() <= 0) {
                scene()->removeItem(monster);
            }

            destroy(); // supprime le projectile
            return;
        }
    }
}

void Projectile::destroy() {
    moveTimer->stop();       // Ajoutez ces deux lignes
    lifespanTimer->stop();
    if (scene()) scene()->removeItem(this);
    deleteLater();
}
void Projectile::setDamage(int d){
    damage = d;
}
int Projectile::getDamage(){
    return damage;
}


