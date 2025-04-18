#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include <QBrush>
#include <QPen>

class HealthBar : public QGraphicsItemGroup {
public:
    HealthBar(int maxHP);

    void updateHP(int currentHP);

private:
    QGraphicsRectItem* background;
    QGraphicsRectItem* bar;
    int maxHP = 100;
};

#endif //HEALTHBAR_H
