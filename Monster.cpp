#include "Monster.hpp"
class MyScene;


Monster::Monster(Player* myPlayer, MyScene* mainScene, QObject* parent): QObject(parent), HP(1), speed(2), player(myPlayer), attackCooldown(1000), mainScene(mainScene){
    for (int i = 0; i < 5; ++i) {
        QSoundEffect* sound = new QSoundEffect(this);
        sound->setSource(QUrl("qrc:/assets/hitMonster.wav"));
        sound->setLoopCount(1);
        sound->setVolume(0.2);
        hitSounds.append(sound);
    }
    lastAttackTime.start();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
    this->move();   // déplacement du monstre
    this->attack(); // attaque du monstre si collision
});
    timer->start(50);//déplacement toute les 50ms
}
int Monster::getSpeed() const {
    return speed;
}
int Monster::getValueScore() const {
    return valueScore;
}
void Monster::setValueScore(int s) {
    valueScore = s;
}
int Monster::getHP() const {
    return HP;
}
int Monster::getDamage() const {
    return damage;
}
int Monster::getAttackCooldown() const {
    return attackCooldown;
}
int Monster::getSpriteSize() const {
    return spriteSize;
}
void Monster::setSpeed(int s) {
    speed = s;
}

void Monster::setHP(int h) {
    if (h <= 0) {
        HP = 0;
        if (mainScene) {
            mainScene->destroyMonster(this); // Appelle directement
        }
    } else {
        HP = h;
    }
}


void Monster::setDamage(int d) {
    damage = d;
}
void Monster::setAttackCooldown(int c) {
    attackCooldown = c;
}
void Monster::setSpriteSize(int size) {
    spriteSize = size;
}
void Monster::move() {
    if (!player) {
        return;
    }

    QPointF monsterPos = this->pos();  // position du monstre
    QPointF playerPos = player->pos(); // position du joueur

    qreal dx = playerPos.x() - monsterPos.x();  // différence en x
    qreal dy = playerPos.y() - monsterPos.y();  // différence en y

    // Calcul des distances horizontale et verticale
    qreal absDx = qAbs(dx);
    qreal absDy = qAbs(dy);

    // Choisir la direction la plus proche
    if (absDx > absDy) {
        // Déplacement horizontal (gauche/droite)
        if (dx > 0) {
            // Aller à droite
            setPixmap(spriteRight->scaled(spriteSize, spriteSize, Qt::KeepAspectRatio, Qt::FastTransformation));
            this->moveBy(speed, 0);
        } else {
            // Aller à gauche
            setPixmap(spriteLeft->scaled(spriteSize, spriteSize, Qt::KeepAspectRatio, Qt::FastTransformation));
            this->moveBy(-speed, 0);
        }
    } else {
        // Déplacement vertical (haut/bas)
        if (dy > 0) {
            // Aller vers le bas
            setPixmap(spriteDown->scaled(spriteSize, spriteSize, Qt::KeepAspectRatio, Qt::FastTransformation));
            this->moveBy(0, speed);
        } else {
            // Aller vers le haut
            setPixmap(spriteUp->scaled(spriteSize, spriteSize, Qt::KeepAspectRatio, Qt::FastTransformation));
            this->moveBy(0, -speed);
        }
    }
}


void Monster::attack() {
    if (!player) return;
    if (this->scene()) {
        if (this->collidesWithItem(player)) {
            if (lastAttackTime.elapsed() >= attackCooldown) {
                int newHP = player->getHP() - damage;
                player->setHP(newHP);
                if (player->getHealthBar()) {
                    player->getHealthBar()->updateHP(newHP);
                }
                qDebug() << "Attaque ! HP joueur :" << player->getHP();
                QSoundEffect* currentSound = hitSounds[currentHitSoundIndex];
                if (currentSound->status() == QSoundEffect::Ready) {
                    currentSound->stop(); // redémarre depuis le début proprement
                    currentSound->play();
                }
                currentHitSoundIndex = (currentHitSoundIndex + 1) % hitSounds.size();

                lastAttackTime.restart(); // reset du cooldown
            }
        }
    }
}

BigMonster::BigMonster(Player* myPlayer, MyScene* ms, QObject* parent)
    : Monster(myPlayer, ms, parent)
{
    setSpeed(2);
    setHP(90);
    setValueScore(500);
    setDamage(25);
    setAttackCooldown(2500);
    setSpriteSize(45);
    spriteUp = new QPixmap(":/assets/BigMonster_rear.png");
    spriteDown = new QPixmap(":/assets/BigMonster_front.png");
    spriteLeft = new QPixmap(":/assets/BigMonster_left.png");
    spriteRight = new QPixmap(":/assets/BigMonster_right.png");
    // Vérification des sprites
    if (spriteUp->isNull() || spriteDown->isNull() || spriteLeft->isNull() || spriteRight->isNull()) {
        qWarning("Erreur : un ou plusieurs sprites sont introuvables !");
    }
    setPixmap(spriteUp->scaled(spriteSize, spriteSize, Qt::KeepAspectRatio, Qt::FastTransformation));
}

SmallMonster::SmallMonster(Player* myPlayer, MyScene* ms, QObject* parent)
    : Monster(myPlayer, ms, parent)
{
    setSpeed(3);
    setHP(60);
    setValueScore(250);
    setDamage(15);
    setSpriteSize(30);
    spriteUp = new QPixmap(":/assets/SmallMonster_rear.png");
    spriteDown = new QPixmap(":/assets/SmallMonster_front.png");
    spriteLeft = new QPixmap(":/assets/SmallMonster_left.png");
    spriteRight = new QPixmap(":/assets/SmallMonster_right.png");
    // Vérification des sprites
    if (spriteUp->isNull() || spriteDown->isNull() || spriteLeft->isNull() || spriteRight->isNull()) {
        qWarning("Erreur : un ou plusieurs sprites sont introuvables !");
    }
    setPixmap(spriteUp->scaled(spriteSize, spriteSize, Qt::KeepAspectRatio, Qt::FastTransformation));
}
ShooterMonster::ShooterMonster(Player* player, MyScene* scene, QObject* parent)
    : Monster(player, scene, parent)
{
    setSpeed(1);
    setHP(40);
    setDamage(10);
    setAttackCooldown(2000);
    setSpriteSize(35);

    spriteUp = new QPixmap(":/assets/RangedMonster_up.png");
    spriteDown = new QPixmap(":/assets/RangedMonster_down.png");
    spriteLeft = new QPixmap(":/assets/RangedMonster_left.png");
    spriteRight = new QPixmap(":/assets/RangedMonster_right.png");

    if (spriteUp->isNull() || spriteDown->isNull() || spriteLeft->isNull() || spriteRight->isNull()) {
        qWarning("Erreur : sprites manquants pour RangedMonster !");
    }

    setPixmap(spriteDown->scaled(spriteSize, spriteSize));
}

void ShooterMonster::attack() {
    if (!player) return;
    if (lastAttackTime.elapsed() >= attackCooldown) {
        shootAtPlayer();
        lastAttackTime.restart();
    }
}

void ShooterMonster::shootAtPlayer() {
    QPointF origin = this->pos();
    QPointF target = player->pos();
    QPointF direction = (target - origin);
    qreal length = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());

    if (length == 0) return;

    QPointF velocity = (direction / length) * 5.0; // 5 pixels/frame

    Projectile* proj = new Projectile(velocity);
    proj->setPos(this->pos());

    if (this->scene()) {
        this->scene()->addItem(proj);
    }
}
Monster::~Monster() {
    delete spriteUp;
    spriteUp = nullptr;
    delete spriteDown;
    spriteDown = nullptr;
    delete spriteLeft;
    spriteLeft = nullptr;
    delete spriteRight;
    spriteRight = nullptr;
}

void Monster::pause() {
    if (timer && timer->isActive()) {
        timer->stop();
    }
}

void Monster::resume() {
    if (timer && !timer->isActive()) {
        timer->start(50);
    }
}