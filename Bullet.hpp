// Bullet.hpp
#ifndef BULLET_HPP
#define BULLET_HPP

#include <QGraphicsRectItem>
#include <QTimer>
#include <QObject>
#include <QPointF>

class Bullet : public QObject, public QGraphicsRectItem {
    Q_OBJECT

public:
    Bullet(QPointF startPos, QPointF targetPos, QGraphicsItem* parent = nullptr);

private slots:
    void move();

private:
    QTimer* timer;
    QPointF velocity;
    qreal speed;
};

#endif // BULLET_HPP
