// bullet.hpp

#ifndef BULLET_HPP
#define BULLET_HPP

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPointF>

class Projectile : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Projectile(const QPointF& velocity, QGraphicsItem* parent = nullptr);

private slots:
    void move();
    void destroy();

private:
    QTimer* moveTimer;
    QTimer* lifespanTimer;
    QPointF velocity;

    void applyRotation(); // 🔥
};

#endif // BULLET_HPP
