#ifndef BULLET_HPP
#define BULLET_HPP

#include <QGraphicsObject>
#include <QTimer>
#include <QPointF>
#include <QPixmap>
#include "Monster.hpp"

class Projectile : public QGraphicsObject {
    Q_OBJECT

public:
    Projectile(const QPointF& velocity, QGraphicsItem* parent = nullptr);
    virtual ~Projectile();

    void setDamage(int d);
    int getDamage();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override;

    private slots:
    void move();
    void destroy();

private:
    QTimer* moveTimer;
    QTimer* lifespanTimer;
    QPointF velocity;
    int damage;
    QPixmap sprite;

    void applyRotation();
};

#endif // BULLET_HPP
