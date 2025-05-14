#include "Player.hpp"
#include <QDebug>
#include "bullet.hpp"
#include <cmath>
#include "MyScene.hpp"

void Player::shoot(const QPointF& targetPos) {
    QPointF direction = targetPos - pos();
    qreal length = std::hypot(direction.x(), direction.y());
    if (length == 0) return;

    const qreal projectileSpeed = 15.0;
    QPointF velocity = direction * (projectileSpeed / length);

    Projectile* projectile = new Projectile(velocity);
    QPointF playerCenter = pos() + QPointF(boundingRect().width() / 2, boundingRect().height() / 2);
    projectile->setPos(playerCenter);
    scene()->addItem(projectile);
    // Ne plus appeler setFocus() ici
}

Player::Player(MainWindow* mw, MyScene* scene, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), speed(2), dx(0), dy(0), mainScene(scene), mainWindow(mw) {
    setHP(100);

    spriteUp = QPixmap(":/assets/nils_rear.png");
    spriteDown = QPixmap(":/assets/nils_front.png");
    spriteLeft = QPixmap(":/assets/nils_left.png");
    spriteRight = QPixmap(":/assets/nils_right.png");

    if (spriteUp.isNull() || spriteDown.isNull() || spriteLeft.isNull() || spriteRight.isNull()) {
        qWarning("Erreur : un ou plusieurs sprites sont introuvables !");
    }

    setPixmap(spriteDown.scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation));

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Player::updatePosition);
    movementTimer->start(16);

    healthBar = new HealthBar(100, this);
}

void Player::keyPressEvent(QKeyEvent* event) {
    pressedKeys.insert(event->key());

    dx = 0;
    dy = 0;

    if (pressedKeys.contains(Qt::Key_Q)) dx -= speed;
    if (pressedKeys.contains(Qt::Key_D)) dx += speed;
    if (pressedKeys.contains(Qt::Key_Z)) dy -= speed;
    if (pressedKeys.contains(Qt::Key_S)) dy += speed;

    if (dx != 0 && dy != 0) {
        dx *= 0.7071;
        dy *= 0.7071;
    }
}

void Player::keyReleaseEvent(QKeyEvent* event) {
    pressedKeys.remove(event->key());

    dx = 0;
    dy = 0;

    if (pressedKeys.contains(Qt::Key_Q)) dx -= speed;
    if (pressedKeys.contains(Qt::Key_D)) dx += speed;
    if (pressedKeys.contains(Qt::Key_Z)) dy -= speed;
    if (pressedKeys.contains(Qt::Key_S)) dy += speed;

    if (dx != 0 && dy != 0) {
        dx *= 0.7071;
        dy *= 0.7071;
    }
}

void Player::updatePosition() {
    moveBy(dx, dy);

    if (dx < 0) {
        setPixmap(spriteLeft.scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation));
    } else if (dx > 0) {
        setPixmap(spriteRight.scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation));
    } else if (dy < 0) {
        setPixmap(spriteUp.scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation));
    } else if (dy > 0) {
        setPixmap(spriteDown.scaled(40, 40, Qt::KeepAspectRatio, Qt::FastTransformation));
    }

    QGraphicsView* myview = mainWindow->getView();
    if (myview) {
        myview->centerOn(this);
    }
}

void Player::setSpeed(int s) {
    speed = s;
}

int Player::getSpeed() const {
    return speed;
}

int Player::getHP() const {
    return HP;
}

void Player::setHP(int h) {
    if (h <= 0) {
        HP = 0;
        alive = false;
        if (mainWindow) {
            mainWindow->die();
        }
    } else {
        HP = h;
    }
}

void Player::setMainWindow(MainWindow* mw) {
    mainWindow = mw;
}

HealthBar* Player::getHealthBar() const {
    return healthBar;
}

bool Player::isAlive() const {
    return alive;
}

void Player::focusOutEvent(QFocusEvent* event) {
    qDebug() << "Focus perdu — récupération";
    setFocus();
    QGraphicsPixmapItem::focusOutEvent(event);
}

Player::~Player() {
    // Rien à faire ici, les enfants sont supprimés automatiquement
}
