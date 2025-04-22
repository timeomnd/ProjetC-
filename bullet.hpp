// bullet.hpp

#ifndef BULLET_HPP
#define BULLET_HPP

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPointF>
#include "Monster.hpp"

class Projectile : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Projectile(const QPointF& velocity, QGraphicsItem* parent = nullptr);
    void setDamage(int d);
    int getDamage();
private slots:
    void move();
    void destroy();


private:
    QTimer* moveTimer;
    QTimer* lifespanTimer;
    QPointF velocity;
    int damage;

    void applyRotation(); // 🔥
};

#endif // BULLET_HPP
