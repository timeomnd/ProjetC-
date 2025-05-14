#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>
#include <QSet>
#include "HealthBar.hpp"
#include "MainWindow.hpp"

class MainWindow;
class MyScene;

class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Player(MainWindow* mw, MyScene* scene, QGraphicsItem* parent = nullptr);
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void setSpeed(int s);
    void setHP(int h);
    void setMainWindow(MainWindow* mw);
    int getSpeed() const;
    int getHP() const;
    bool isAlive() const;
    HealthBar* getHealthBar() const;
    void gunShoot(const QPointF& targetPos);
    void shotgunShoot(const QPointF& targetPos);
    virtual ~Player();

protected:
    void focusOutEvent(QFocusEvent* event) override;

private slots:
    void updatePosition();

private:
    int speed;
    int HP;
    int dx;
    int dy;
    bool alive = true;
    HealthBar* healthBar;
    MainWindow* mainWindow = nullptr;
    MyScene* mainScene = nullptr;
    QTimer* movementTimer;
    QSet<int> pressedKeys;
    QPixmap spriteUp;
    QPixmap spriteDown;
    QPixmap spriteLeft;
    QPixmap spriteRight;
};

#endif // PLAYER_HPP
