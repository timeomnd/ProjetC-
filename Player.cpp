#include "Player.hpp"
#include <QDebug>
#include <cmath>
#include "MyScene.hpp"

// Constantes
const int INITIAL_HP = 100;
const int SPRITE_SIZE = 40;
const qreal DIAGONAL_SPEED_FACTOR = 0.8; // 1/√2
const int COLLISION_BOX_SIZE = 4;
const int MOVEMENT_TIMER_INTERVAL_MS = 16; // ~60 FPS

Player::Player(MainWindow* mw, MyScene* scene, Map* map, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), speed(2), dx(0), dy(0),
      mainScene(scene), mainWindow(mw), map(map), alive(true), initialSpeed(speed) {

    setHP(INITIAL_HP);
    healthBar = new HealthBar(INITIAL_HP, this);

    setBoundingRegionGranularity(0.85);

    gun = new Gun(mainScene);
    shotgun = new Shotgun(mainScene);
    currentWeapon = gun;

    spriteUp = QPixmap(":/assets/nils_rear.png").scaled(49, 49, Qt::KeepAspectRatio, Qt::FastTransformation);
    spriteDown = QPixmap(":/assets/nils_front.png").scaled(49, 49, Qt::KeepAspectRatio, Qt::FastTransformation);
    spriteLeft = QPixmap(":/assets/nils_left.png").scaled(49, 49, Qt::KeepAspectRatio, Qt::FastTransformation);
    spriteRight = QPixmap(":/assets/nils_right.png").scaled(49, 49, Qt::KeepAspectRatio, Qt::FastTransformation);

    if (spriteUp.isNull() || spriteDown.isNull() || spriteLeft.isNull() || spriteRight.isNull()) {
        qWarning("Erreur : un ou plusieurs sprites sont introuvables !");
    }

    setPixmap(spriteDown);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    // Chargement des sons de dégâts (damage1.wav à damage5.wav)
    for (int i = 1; i <= 5; ++i) {
        QSoundEffect* sound = new QSoundEffect(this);
        sound->setSource(QUrl(QString("qrc:/assets/damage%1.wav").arg(i)));
        sound->setLoopCount(1);
        sound->setVolume(0.2);
        hitSounds.append(sound);
    }

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Player::updatePosition);
    movementTimer->start(MOVEMENT_TIMER_INTERVAL_MS);
}



QRectF Player::getCollisionBounds() const {

    const QRectF spriteBounds = boundingRect();
    const qreal offsetX = (spriteBounds.width()) / 2 - 5 ;
    const qreal offsetY = (spriteBounds.height()) / 2 + 4;

    return QRectF(offsetX, offsetY, 10, 14);
}

bool Player::checkTileCollision(const QPointF& newPos) const {
    if (!map) return false;

    QRectF playerBounds = getCollisionBounds().translated(newPos);
    

    for (const QRectF& rect : map->getCollisionRects()) {
        if (playerBounds.intersects(rect)) {
            return true;
        }
    }
    return false;
}

void Player::updateMovementVector() {
    dx = 0;
    dy = 0;

    if (pressedKeys.contains(Qt::Key_Q)) dx -= speed;
    if (pressedKeys.contains(Qt::Key_D)) dx += speed;
    if (pressedKeys.contains(Qt::Key_Z)) dy -= speed;
    if (pressedKeys.contains(Qt::Key_S)) dy += speed;


    if (dx != 0 && dy != 0) {
        dx *= DIAGONAL_SPEED_FACTOR;
        dy *= DIAGONAL_SPEED_FACTOR;
    }
}

void Player::keyPressEvent(QKeyEvent* event) {
    pressedKeys.insert(event->key());


    switch(event->key()) {
        case Qt::Key_X: switchWeapon(1); break;
        case Qt::Key_C: switchWeapon(2); break;
        break;
        default: break;
    }

    updateMovementVector();
}

void Player::keyReleaseEvent(QKeyEvent* event) {
    pressedKeys.remove(event->key());
    updateMovementVector();
}

void Player::updatePosition() {
     QPointF newPos = pos() + QPointF(dx, dy);

    if (!checkTileCollision(newPos)) {
        setPos(newPos); 
    }


    bool collisionX = checkTileCollision(QPointF(newPos.x(), pos().y()));
    bool collisionY = checkTileCollision(QPointF(pos().x(), newPos.y()));

    if(dx != 0 && dy != 0) {

        QPointF tempPos = pos() + QPointF(dx, 0);
        if(!checkTileCollision(tempPos)) {
            setX(tempPos.x());
        }


        tempPos = pos() + QPointF(0, dy);
        if(!checkTileCollision(tempPos)) {
            setY(tempPos.y());
        }
    } else {

        if(!collisionX) setX(newPos.x());
        if(!collisionY) setY(newPos.y());
    }


    if (dx < 0) setPixmap(spriteLeft);
    else if (dx > 0) setPixmap(spriteRight);
    else if (dy < 0) setPixmap(spriteUp);
    else if (dy > 0) setPixmap(spriteDown);


    if (mainWindow->getView()) mainWindow->getView()->centerOn(this);
}

void Player::playRandomHitSound() {
    if (!hitSounds.isEmpty()) {
        int index = QRandomGenerator::global()->bounded(hitSounds.size());
        hitSounds[index]->play();
    }
}

void Player::setHP(int h) {
    HP = qBound(0, h, INITIAL_HP);
    alive = (HP > 0);

    if (!alive) {
        mainScene->getScoreManager()->saveScore();
        QCoreApplication::processEvents();
        if (mainWindow) mainWindow->die();
    }
}

void Player::setInitalSpeed(int s) {
    initialSpeed = s;
}
int Player::getInitalSpeed() const {
    return initialSpeed;
}


int Player::getHP() const {
    return HP;
}

HealthBar* Player::getHealthBar() const {
    return healthBar;
}

Weapon* Player::getCurrentWeapon() const{
    return currentWeapon;
}

bool Player::isAlive() const {
    return alive;
}


void Player::focusOutEvent(QFocusEvent* event) {
    setFocus();
    QGraphicsPixmapItem::focusOutEvent(event);
}

void Player::switchWeapon(int weaponType) {
    currentWeapon = (weaponType == 1) ? static_cast<Weapon*>(gun) : shotgun;
}


void Player::pause() {
    if (movementTimer) movementTimer->stop();
}

void Player::resume() {
    if (movementTimer) movementTimer->start(MOVEMENT_TIMER_INTERVAL_MS);
}
int Player::getSpeed() const {
    return speed;
}

void Player::setSpeed(int newSpeed) {
    speed = newSpeed;
}

Player::~Player() {
    delete gun;
    delete shotgun;
}
void Player::setCanShoot(bool c) {
    canShoot = c;
}
bool Player::getCanShoot() const {
    return canShoot;
}



MyScene* Player::getMyScene() const {
    return mainScene;
}
