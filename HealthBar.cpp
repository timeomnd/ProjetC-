#include "HealthBar.hpp"
HealthBar::HealthBar(int maxHP)
    : maxHP(maxHP)
{
    background = new QGraphicsRectItem(0, 0, 300, 20); //taille de la barre de vie
    background->setBrush(Qt::red);
    background->setPen(Qt::NoPen);

    bar = new QGraphicsRectItem(0, 0, 300, 20); //taille de la vie restante
    bar->setBrush(Qt::green);
    bar->setPen(Qt::NoPen);

    addToGroup(background);
    addToGroup(bar);
}

void HealthBar::updateHP(int currentHP) {
    float ratio = qMax(0.0, float(currentHP) / maxHP);
    bar->setRect(0, 0, 300 * ratio, 20); //mise a jour de la partie restante de la vie (en vert)
}
