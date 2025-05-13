#include "bullet.hpp"
#include <QGraphicsScene>
#include <QtMath>
#include <QPainter>

Projectile::Projectile(const QPointF& velocity, QGraphicsItem* parent)
    : QGraphicsObject(parent), velocity(velocity), sprite(":/assets/bullet.png") {

    setDamage(10);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
    applyRotation();

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Projectile::move);
    moveTimer->start(16);

    lifespanTimer = new QTimer(this);
    connect(lifespanTimer, &QTimer::timeout, this, &Projectile::destroy);
    lifespanTimer->start(1000);
}

void Projectile::applyRotation() {
    qreal angle = std::atan2(velocity.y(), velocity.x()) * 180 / M_PI;
    setRotation(angle);
}

QRectF Projectile::boundingRect() const {
    return QRectF(-sprite.width() / 2, -sprite.height() / 2, sprite.width(), sprite.height());
}

void Projectile::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->drawPixmap(boundingRect().topLeft(), sprite);
}

void Projectile::move() {
    setPos(pos() + velocity);

    if (!scene() || !scene()->sceneRect().contains(pos())) {
        destroy();
        return;
    }

    for (QGraphicsItem* item : collidingItems()) {
        Monster* monster = dynamic_cast<Monster*>(item);
        if (monster) {
            monster->setHP(monster->getHP() - getDamage());
            if (monster->getHP() <= 0 && scene()) {
                scene()->removeItem(monster);
            }
            destroy();
            return;
        }
    }
}

void Projectile::destroy() {
    moveTimer->stop();
    lifespanTimer->stop();
    if (scene()) scene()->removeItem(this);
    this->deleteLater();
}

Projectile::~Projectile() {
    // Les timers sont des enfants, donc supprimés automatiquement
}

void Projectile::setDamage(int d) {
    damage = d;
}
int Projectile::getDamage() {
    return damage;
}
