#ifndef WEAPONS_HPP
#define WEAPONS_HPP

#include <QObject>
#include <QPointF>
#include <QGraphicsScene>
#include <QSoundEffect>
#include <QElapsedTimer>
#include <QVector>

class Projectile;

class Weapon : public QObject {
    Q_OBJECT
public:
    Weapon(QGraphicsScene* scene, QObject* parent = nullptr);
    virtual ~Weapon();
    virtual void shoot(const QPointF& sourcePos, const QPointF& targetPos) = 0;
    virtual bool canShoot() const = 0;
    void setScene(QGraphicsScene* scene);

protected:
    QGraphicsScene* scene;
    QElapsedTimer* cooldownTimer;
    int cooldownMs;
    QVector<QSoundEffect*> sounds;
    int currentSoundIndex;
    static const int SOUND_INSTANCES = 5;
};

class Gun : public Weapon {
public:
    Gun(QGraphicsScene* scene, QObject* parent = nullptr);
    void shoot(const QPointF& sourcePos, const QPointF& targetPos) override;
    bool canShoot() const override { return true; } // Pas de cooldown pour le gun

};

class Shotgun : public Weapon {
public:
    Shotgun(QGraphicsScene* scene, QObject* parent = nullptr);
    void shoot(const QPointF& sourcePos, const QPointF& targetPos) override;
    bool canShoot() const override;
};

#endif // WEAPONS_HPP