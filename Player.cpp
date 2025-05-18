#include "Player.hpp"
#include <QDebug>
#include <cmath>
#include "MyScene.hpp"

// Constantes
const int INITIAL_HP = 100;
const int SPRITE_SIZE = 40;
const qreal DIAGONAL_SPEED_FACTOR = 0.7071; // 1/√2
const int COLLISION_BOX_SIZE = 4;
const int MOVEMENT_TIMER_INTERVAL_MS = 16; // ~60 FPS

Player::Player(MainWindow* mw, MyScene* scene, Map* map, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), speed(2), dx(0), dy(0), 
      mainScene(scene), mainWindow(mw), map(map), alive(true) {
    

    setHP(INITIAL_HP);
    healthBar = new HealthBar(INITIAL_HP, this); 


    setBoundingRegionGranularity(0.85);


    gun = new Gun(mainScene);
    shotgun = new Shotgun(mainScene);
    currentWeapon = gun;


    spriteUp = QPixmap(":/assets/nils_rear.png").scaled(SPRITE_SIZE, SPRITE_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    spriteDown = QPixmap(":/assets/nils_front.png").scaled(SPRITE_SIZE, SPRITE_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    spriteLeft = QPixmap(":/assets/nils_left.png").scaled(SPRITE_SIZE, SPRITE_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    spriteRight = QPixmap(":/assets/nils_right.png").scaled(SPRITE_SIZE, SPRITE_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if (spriteUp.isNull() || spriteDown.isNull() || spriteLeft.isNull() || spriteRight.isNull()) {
        qWarning("Erreur : un ou plusieurs sprites sont introuvables !");
    }

    setPixmap(spriteDown);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();


    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Player::updatePosition);
    movementTimer->start(MOVEMENT_TIMER_INTERVAL_MS);
}


QRectF Player::getCollisionBounds() const {
    const QRectF spriteBounds = boundingRect();
    const qreal offsetX = (spriteBounds.width() - COLLISION_BOX_SIZE) / 2;
    const qreal offsetY = (spriteBounds.height() - COLLISION_BOX_SIZE) / 2;
    
    return QRectF(pos().x() + offsetX, pos().y() + offsetY, COLLISION_BOX_SIZE, COLLISION_BOX_SIZE);
}

bool Player::checkTileCollision(const QPointF& newPos) const {
    QRectF playerBounds = getCollisionBounds().translated(newPos - pos());
    

    const QPointF centerPoint = playerBounds.center();
    const QPoint centerTile(floor(centerPoint.x() / 16), floor(centerPoint.y() / 16));
    
    if(map->getCollisionRects().contains(QRectF(centerTile.x() * 16, centerTile.y() * 16, 16, 16))) {
        return true;
    }


    const QPointF points[4] = {
        playerBounds.topLeft(),
        playerBounds.topRight(),
        playerBounds.bottomLeft(),
        playerBounds.bottomRight()
    };

    for(const QPointF& point : points) {
        const QPoint tilePos(floor(point.x() / 16), floor(point.y() / 16)); // Précision améliorée
        if(map->getCollisionRects().contains(QRectF(tilePos.x() * 16, tilePos.y() * 16, 16, 16))) {
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
        default: break;
    }

    updateMovementVector();
}

void Player::keyReleaseEvent(QKeyEvent* event) {
    pressedKeys.remove(event->key());
    updateMovementVector();
}


void Player::updatePosition() {
    QPointF movement(dx, dy);
    QPointF newPos = pos() + movement;

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


void Player::setHP(int h) {
    HP = qBound(0, h, INITIAL_HP);
    alive = (HP > 0);

    if (!alive) {
        mainScene->getScoreManager()->saveScore();
        QCoreApplication::processEvents();
        if (mainWindow) mainWindow->die();
    }
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

Player::~Player() {
    delete gun;
    delete shotgun;
}