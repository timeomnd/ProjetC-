#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>
#include "HealthBar.hpp"
#include <QSet>

class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

    public:
    Player(QGraphicsItem* parent = nullptr);
    void die();
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void setSpeed(int s);
    void setHP(int h);
    int getSpeed() const;
    int getHP()const;
    bool isAlive() const;
    HealthBar* getHealthBar() const;
    void shoot(const QPointF& targetPos); // Declaration added here
private slots:
    void updatePosition();
private:
    int speed;
    int HP;
    int dx;
    int dy;
    bool alive = true;
    HealthBar* healthBar;
    QTimer* movementTimer;
    QSet<int> pressedKeys;
    QPixmap spriteUp;    // Sprite pour la direction haut
    QPixmap spriteDown;  // Sprite pour la direction bas
    QPixmap spriteLeft;  // Sprite pour la direction gauche
    QPixmap spriteRight; // Sprite pour la direction droite
};

#endif // PLAYER_HPP