#include "HealthBar.hpp"
#include "Player.hpp"
HealthBar::HealthBar(int maxHP, Player* parent)
    : maxHP(maxHP), player(parent)
{
    background = new QGraphicsRectItem(0, 0, 100, 10); //taille de la barre de vie
    background->setBrush(Qt::red);
    background->setPen(Qt::NoPen);

    bar = new QGraphicsRectItem(0, 0, 100, 10); //taille de la vie restante
    bar->setBrush(Qt::green);
    bar->setPen(Qt::NoPen);

    addToGroup(background);
    addToGroup(bar);
}

void HealthBar::updateHP(int currentHP) {
    float ratio = qMax(0.0, float(currentHP) / maxHP);
    bar->setRect(0, 0, 100 * ratio, 10); //mise a jour de la partie restante de la vie (en vert)
}
