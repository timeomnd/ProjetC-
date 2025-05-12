#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include <QBrush>
#include <QPen>

#include "Player.hpp"

class HealthBar : public QGraphicsItemGroup {
public:
    HealthBar(int maxHP, Player* parent);
    void updateHP(int currentHP);

private:
    QGraphicsRectItem* background;
    QGraphicsRectItem* bar;
    Player* player;
    int maxHP = 100;
};

#endif //HEALTHBAR_H
