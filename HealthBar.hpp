// HealthBar.hpp
#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include <QBrush>
#include <QPen>

// Déclaration anticipée de Player, au lieu de l'inclure ici
class Player;

class HealthBar : public QGraphicsItemGroup {
public:
    HealthBar(int maxHP, Player* parent);
    void updateHP(int currentHP);

private:
    QGraphicsRectItem* background;
    QGraphicsRectItem* bar;
    Player* player;  // Utilisation de Player* sans inclure Player.hpp ici
    int maxHP = 100;
};

#endif //HEALTHBAR_H
