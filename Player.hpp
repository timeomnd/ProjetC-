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
#include "Weapons.hpp"


class MainWindow;
class MyScene;
class Map;

class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Player(MainWindow* mw, MyScene* scene, Map* map, QGraphicsItem* parent = nullptr);
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void setSpeed(int s);
    void setHP(int h);
    void playRandomHitSound();
    void loadAnimations();
    void setMainWindow(MainWindow* mw);
    int getSpeed() const;
    int getHP() const;
    bool isAlive() const;
    void setInitalSpeed(int s);
    int getInitalSpeed() const;
    MyScene* getMyScene() const;
    HealthBar* getHealthBar() const;
    virtual ~Player();

    Weapon* getCurrentWeapon() const;
    void switchWeapon(int weaponType);
    void pause();
    void resume();

    bool checkTileCollision(const QPointF& newPos) const;
    QRectF getCollisionBounds() const;
    void updateMovementVector();
    void setCanShoot(bool c);
    bool getCanShoot() const;

protected:
    void focusOutEvent(QFocusEvent* event) override;

private slots:
    void updatePosition();

private:
    int speed;
    int initialSpeed;
    int HP;
    int dx;
    int dy;
    int frameIndex = 0;
    int frameCounter = 0;
    const int frameDelay = 10;
    bool canShoot = true;
    QString lastDirection = "down"; // Pour savoir dans quel sens regarder quand le joueur s'arrête
    bool alive = true;
    HealthBar* healthBar;
    MainWindow* mainWindow = nullptr;
    MyScene* mainScene = nullptr;
    QVector<QSoundEffect*> hitSounds;
    QTimer* movementTimer;
    QSet<int> pressedKeys;
    QPixmap* spriteUp;
    QPixmap* spriteDown;
    QPixmap* spriteLeft;
    QPixmap* spriteRight;

    QVector<QPixmap*> animationUpMove;
    QVector<QPixmap*> animationDownMove;
    QVector<QPixmap*> animationLeftMove;
    QVector<QPixmap*> animationRightMove;
    Map* map;



    // Armes
    Weapon* currentWeapon;
    Gun* gun;
    Shotgun* shotgun;
};

#endif // PLAYER_HPP