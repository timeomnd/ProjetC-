#include "Weapons.hpp"
#include "bullet.hpp"
#include <QDebug>
#include <QLineF>
#include <QtMath>

Weapon::Weapon(QGraphicsScene* scene, QObject* parent) 
    : QObject(parent), scene(scene), cooldownTimer(nullptr), cooldownMs(0), currentSoundIndex(0) {
    cooldownTimer = new QElapsedTimer();
    cooldownTimer->start();
}

Weapon::~Weapon() {
    delete cooldownTimer;
    if (!sounds.isEmpty()) {
        for (QSoundEffect* sound : sounds) {
            delete sound;
        }
    }
}

void Weapon::setScene(QGraphicsScene* scene) {
    this->scene = scene;
}

Gun::Gun(QGraphicsScene* scene, QObject* parent) 
    : Weapon(scene, parent) {
    for (int i = 0; i < SOUND_INSTANCES; ++i) {
        QSoundEffect* sound = new QSoundEffect(this);
        sound->setSource(QUrl("qrc:/assets/gun.wav"));
        sound->setLoopCount(1);
        sound->setVolume(0.2);
        sounds.append(sound);
    }
}

void Gun::shoot(const QPointF& sourcePos, const QPointF& targetPos) {
    QPointF direction = targetPos - sourcePos;
    qreal length = std::hypot(direction.x(), direction.y());
    if (length == 0) return;

    const qreal projectileSpeed = 12.0;
    QPointF velocity = direction * (projectileSpeed / length);
    // Jouer le son
    QSoundEffect* sound = sounds[currentSoundIndex];
    if (sound->status() == QSoundEffect::Ready) {
        sound->stop();
        sound->play();
    }
    currentSoundIndex = (currentSoundIndex + 1) % sounds.size();
    QPixmap* sprite = new QPixmap(":/assets/bullet.png");
    Projectile* projectile = new Projectile(*sprite, velocity);
    projectile->setPos(sourcePos);
    scene->addItem(projectile);
}

Shotgun::Shotgun(QGraphicsScene* scene, QObject* parent)
    : Weapon(scene, parent) {
    cooldownMs = 600;

    // Charger plusieurs sons pour éviter les conflits
    for (int i = 0; i < SOUND_INSTANCES; ++i) {
        QSoundEffect* sound = new QSoundEffect(this);
        sound->setSource(QUrl("qrc:/assets/pompe.wav"));
        sound->setLoopCount(1);
        sound->setVolume(0.2);
        sounds.append(sound);
    }
}

bool Shotgun::canShoot() const {
    return cooldownTimer->elapsed() >= cooldownMs;
}

void Shotgun::shoot(const QPointF& sourcePos, const QPointF& targetPos) {
    if (!canShoot()) return;
    cooldownTimer->restart();

    QLineF directionLine(sourcePos, targetPos);
    qreal baseAngle = directionLine.angle();

    const qreal projectileSpeed = 8.5;
    const QList<qreal> angleOffsets = { -15, -5, 5, 15 };

    // Jouer le son
    QSoundEffect* sound = sounds[currentSoundIndex];
    if (sound->status() == QSoundEffect::Ready) {
        sound->stop();
        sound->play();
    }
    currentSoundIndex = (currentSoundIndex + 1) % sounds.size();

    for (qreal offset : angleOffsets) {
        qreal angle = baseAngle + offset;
        qreal radian = qDegreesToRadians(-angle);

        QPointF velocity(projectileSpeed * qCos(radian),
                         projectileSpeed * qSin(radian));
        QPixmap* sprite = new QPixmap(":/assets/pompe_bullet.png");
        Projectile* projectile = new Projectile(*sprite,velocity);
        projectile->setPos(sourcePos);
        scene->addItem(projectile);
    }
}