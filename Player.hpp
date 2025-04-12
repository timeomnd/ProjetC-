#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>

class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Player(QGraphicsItem* parent = nullptr);
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void setSpeed(int s);
    int getSpeed() const;

private slots:
    void updatePosition();

private:
    int speed;
    int dx;
    int dy;
    QTimer* movementTimer;
    QPixmap spriteUp;    // Sprite pour la direction haut
    QPixmap spriteDown;  // Sprite pour la direction bas
    QPixmap spriteLeft;  // Sprite pour la direction gauche
    QPixmap spriteRight; // Sprite pour la direction droite
};

#endif // PLAYER_HPP