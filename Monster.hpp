#ifndef MONSTER_H
#define MONSTER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>
#include "Player.hpp"
#include <QTimer>
#include <QPixmap>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QLineF>
#include <QSoundEffect>
#include <QElapsedTimer>
#include "HealthBar.hpp"
#include "MyScene.hpp"
#include "bullet.hpp"
#include <QGraphicsColorizeEffect>

class Player;
class MyScene;

class Monster : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Monster(Player* myPlayer, MyScene* mainScene, Map* map, bool isTemporary = false, QObject* parent = nullptr);
    virtual ~Monster();

    int getSpeed() const;
    int getValueScore() const;
    void setValueScore(int s);
    int getHP() const;
    int getDamage() const;
    int getAttackCooldown() const;
    int getSpriteSize() const;

    void setSpeed(int s);
    void setHP(int h);
    void setDamage(int d);
    void setAttackCooldown(int c);
    void setSpriteSize(int size);

    virtual void move();
    virtual void attack();
    void pause();
    void resume();
    virtual void loadAnimations();
    virtual void updateAnimationFrame();
    void showHitEffect();
    virtual QRectF getCollisionBounds() const;

    virtual bool checkTileCollision(const QPointF& newPos) const = 0;


protected:
    Player* player;
    MyScene* mainScene;
    QTimer* timer;

    int HP;
    int speed;
    int damage;
    int attackCooldown;
    int valueScore = 0;
    int spriteSize = 64;

    QElapsedTimer lastAttackTime;

    QVector<QSoundEffect*> hitSounds;
    int currentHitSoundIndex = 0;

    QTimer* animationTimer = nullptr;
    int currentFrameIndex = 0;
    bool isMoving = false;
    QPixmap* idleSheet;
    QPixmap* moveSheet;

    QVector<QPixmap*> animationUpIdle;
    QVector<QPixmap*> animationDownIdle;
    QVector<QPixmap*> animationLeftIdle;
    QVector<QPixmap*> animationRightIdle;

    QVector<QPixmap*> animationUpMove;
    QVector<QPixmap*> animationDownMove;
    QVector<QPixmap*> animationLeftMove;
    QVector<QPixmap*> animationRightMove;
    
    Map* map; 
    
    


};









class DoctorMonster : public Monster {
    Q_OBJECT
public:
    DoctorMonster(Player* myPlayer, MyScene* ms, Map* map, bool isTemporary = false, QObject* parent = nullptr);
    ~DoctorMonster() override;
    void move() override;
    void attack() override;
    void loadAnimations() override;
    void setIsMoving(bool m);
    bool getIsMoving() const;
    void setIsAttacking(bool a);
    bool getIsAttacking() const;
    void updateAttackAnimation();
    QRectF getCollisionBounds() const;
    bool checkTileCollision(const QPointF& newPos) const; 

private:
    QPixmap* attackSheet;
    QVector<QPixmap*> animationAttackLeft;
    QVector<QPixmap*> animationAttackRight;
    QVector<QPixmap*> animationAttackUp;
    QVector<QPixmap*> animationAttackDown;
    QVector<QPixmap*>* currentAttackAnimation = nullptr;
    int attackFrameIndex = 0;  // index pour animation attaque
    bool isMoving = false;
    bool isAttacking = false;
    QTimer* attackAnimationTimer = nullptr;
};






class GhostMonster : public Monster {
    Q_OBJECT
public:
    GhostMonster(Player* myPlayer, MyScene* ms, Map* map, bool isTemporary = false, QObject* parent = nullptr);
    void attack() override;
    void disableShoot();
    void resetShoot();
    bool checkTileCollision(const QPointF& newPos) const override;
private:
    QTimer* canShootTimer = nullptr;
};




class BirdMonster : public Monster {
    Q_OBJECT
public:
    BirdMonster(Player* myPlayer, MyScene* ms, Map* map, bool isTemporary = false, QObject* parent = nullptr);
    void attack() override;
    void loadAnimations() override;
    void move() override;
    void updateAttackAnimation();
    QRectF getCollisionBounds() const;
    bool checkTileCollision(const QPointF& newPos) const; 
private:
    QPixmap* attackSheet;
    QVector<QPixmap*> animationAttackLeft;
    QVector<QPixmap*> animationAttackRight;
    QVector<QPixmap*> animationAttackUp;
    QVector<QPixmap*> animationAttackDown;
    QVector<QPixmap*>* currentAttackAnimation = nullptr;
    int attackFrameIndex = 0;  // index pour animation attaque
    bool isMoving = false;
    bool isAttacking = false;
    QTimer* attackAnimationTimer = nullptr;

};





class SlimeMonster : public Monster {
    Q_OBJECT
public :
    SlimeMonster(Player* myPlayer, MyScene* ms, Map* map, bool isTemporary = false, QObject* parent = nullptr);
    ~SlimeMonster() override;
    void jump();
    void loadAnimations() override;
    void move() override;
    void attack() override;
    void slow();
    QTimer* getSlowTimer() const;
    public slots :
    void resetSpeed();
    QRectF getCollisionBounds() const;
    bool checkTileCollision(const QPointF& newPos) const; 
    
private:
    QVector<QPixmap*> animationIdle;
    QPixmap* moveLeftSheet;
    QPixmap* moveRightSheet;
    QPixmap* attackSheet;
    QVector<QPixmap*> animationAttack;
    QVector<QPixmap*> animationJumpUp;
    QVector<QPixmap*> animationJumpDown;
    QPixmap* jumpSheet;
    QTimer* jumpTimer = nullptr;
    QTimer* jumpCooldownTimer = nullptr;
    QTimer* slowTimer;
    bool isJumping = false;
    int jumpFrameIndex = 0;
    int currentFrameIndex = 0;
};




class Fireball : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Fireball(Player* player, QPointF startPos, QPointF targetPos, QGraphicsScene* scene, QObject* parent = nullptr);
    ~Fireball();
    void setDamage (int d);
    int getDamage();
    private slots:
        void moveAndAnimate();
        void destroySelf();

private:
    bool initialAnimationDone = false;
    QVector<QPixmap*> animationFrames;
    Player* player;
    int damage;
    int currentHitSoundIndex = 0;
    int currentFrameIndex;
    QTimer* animationTimer;
    QPointF velocity;
    QVector<QSoundEffect*> hitSounds;
    qreal speed = 8.0;
};

#endif // MONSTER_H
