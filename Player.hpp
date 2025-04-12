#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>
#include <QTimer>

class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Player(QGraphicsItem* parent = nullptr);

    void keyPressEvent(QKeyEvent* event) override;
    void setSpeed(int s);
    int getSpeed() const;

private slots:
    void updatePosition();

private:
    int speed;
    int dx;
    int dy;
    QTimer* movementTimer;
};

#endif // PLAYER_HPP
