#include "Monster.hpp"
class MyScene;


Monster::Monster(Player* myPlayer, MyScene* mainScene, QObject* parent): QObject(parent), HP(1), speed(2), player(myPlayer),
attackCooldown(1000), mainScene(mainScene), currentFrameIndex(0), isMoving(false), idleSheet(nullptr), moveSheet(nullptr){
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
    if (!player) return;

    QPointF monsterPos = this->pos();
    QPointF playerPos = player->pos();
    qreal dx = playerPos.x() - monsterPos.x();
    qreal dy = playerPos.y() - monsterPos.y();
    qreal absDx = qAbs(dx);
    qreal absDy = qAbs(dy);

    QVector<QPixmap*>* currentAnimation = nullptr;
    isMoving = true;

    if (absDx > absDy) {
        if (dx > 0) {
            moveBy(speed, 0);
            currentAnimation = isMoving ? &animationRightMove : &animationRightIdle;
        } else {
            moveBy(-speed, 0);
            currentAnimation = isMoving ? &animationLeftMove : &animationLeftIdle;
        }
    } else {
        if (dy > 0) {
            moveBy(0, speed);
            currentAnimation = isMoving ? &animationDownMove : &animationDownIdle;
        } else {
            moveBy(0, -speed);
            currentAnimation = isMoving ? &animationUpMove : &animationUpIdle;
        }
    }

    if (currentAnimation && !currentAnimation->isEmpty()) {
        setPixmap(*(*currentAnimation)[currentFrameIndex]);
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
                if (mainScene->getScoreManager()) {
                    mainScene->getScoreManager()->addPoints(-(this->getDamage())*20); // on enlève des points au score si on se fait toucher
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
void Monster::updateAnimationFrame() {
    currentFrameIndex = (currentFrameIndex + 1) % 4;
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

void Monster::loadAnimations() {
    if (idleSheet->isNull() || moveSheet->isNull()) {
        qWarning("Erreur : sprites introuvables !");
        return;
    }

    int frameWidth = idleSheet->width() / 4;
    int frameHeight = idleSheet->height() / 4;

    for (int i = 0; i < 4; ++i) {
        animationDownIdle.append(new QPixmap(idleSheet->copy(i * frameWidth, 0 * frameHeight, frameWidth, frameHeight)));
        animationLeftIdle.append(new QPixmap(idleSheet->copy(i * frameWidth, 1 * frameHeight, frameWidth, frameHeight)));
        animationRightIdle.append(new QPixmap(idleSheet->copy(i * frameWidth, 2 * frameHeight, frameWidth, frameHeight)));
        animationUpIdle.append(new QPixmap(idleSheet->copy(i * frameWidth, 3 * frameHeight, frameWidth, frameHeight)));

        animationDownMove.append(new QPixmap(moveSheet->copy(i * frameWidth, 0 * frameHeight, frameWidth, frameHeight)));
        animationLeftMove.append(new QPixmap(moveSheet->copy(i * frameWidth, 1 * frameHeight, frameWidth, frameHeight)));
        animationRightMove.append(new QPixmap(moveSheet->copy(i * frameWidth, 2 * frameHeight, frameWidth, frameHeight)));
        animationUpMove.append(new QPixmap(moveSheet->copy(i * frameWidth, 3 * frameHeight, frameWidth, frameHeight)));
    }

    setPixmap(*animationDownIdle[0]);
}
Monster::~Monster() {
    // Supprimer tous les QPixmap* dans chaque QVector et vider les vecteurs
    auto clearPixmaps = [](QVector<QPixmap*>& vec) {
        for (QPixmap* pixmap : vec) {
            delete pixmap;
        }
        vec.clear();
    };

    clearPixmaps(animationUpIdle);
    clearPixmaps(animationDownIdle);
    clearPixmaps(animationLeftIdle);
    clearPixmaps(animationRightIdle);

    clearPixmaps(animationUpMove);
    clearPixmaps(animationDownMove);
    clearPixmaps(animationLeftMove);
    clearPixmaps(animationRightMove);

    // Supprimer le timer si existant (Qt s'en occupe si parent est bien assigné, mais c'est plus safe)
    if (animationTimer) {
        animationTimer->stop();
        delete animationTimer;
        animationTimer = nullptr;
    }
}


GhostMonster::GhostMonster(Player* myPlayer, MyScene* ms, QObject* parent)
    : Monster(myPlayer, ms, parent)
{
    setSpeed(2);
    setHP(50);
    setDamage(5);
    setAttackCooldown(1500);
    setValueScore(300);
    slowTimer = new QTimer(this);
    idleSheet = new QPixmap(":/assets/ghost_idle.png");
    moveSheet = new QPixmap(":/assets/ghost_move.png");
    loadAnimations();

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &GhostMonster::updateAnimationFrame);
    animationTimer->start(150); // 150 ms par frame
}
void GhostMonster::attack(){
    if (!player) return;
    if (this->scene()) {
        if (this->collidesWithItem(player)) {
            if (lastAttackTime.elapsed() >= attackCooldown) {
                int newHP = player->getHP() - damage;
                player->setHP(newHP);
                if (player->getHealthBar()) {
                    player->getHealthBar()->updateHP(newHP);
                }
                if (mainScene->getScoreManager()) {
                    mainScene->getScoreManager()->addPoints(-(this->getDamage())*20); // on enlève des points au score si on se fait toucher
                }
                slow();
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
void GhostMonster::slow() {
    if (player->getSpeed()!=player->getInitalSpeed()) {
        return;
    }
    else {
        player->setSpeed(player->getSpeed()*0.75);
        slowTimer->start(3500); // 3.5 secondes de ralentissement
        connect(slowTimer, &QTimer::timeout, this,&GhostMonster::resetSpeed);  }
}
void GhostMonster::resetSpeed() {
    if (player->getSpeed()!=player->getInitalSpeed()) {
        player->setSpeed(player->getInitalSpeed());
    }
}
QTimer* GhostMonster::getSlowTimer() const {
    return slowTimer;
}



BirdMonster::BirdMonster(Player* myPlayer, MyScene* ms, QObject* parent)
    : Monster(myPlayer, ms, parent)
{
    setSpeed(3);
    setHP(60);
    setDamage(15);
    setAttackCooldown(1200);
    setValueScore(250);
    idleSheet = new QPixmap(":/assets/bird_idle.png");
    moveSheet = new QPixmap(":/assets/bird_move.png");

    loadAnimations();

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &BirdMonster::updateAnimationFrame);
    animationTimer->start(150);
}

DoctorMonster::DoctorMonster(Player* myPlayer, MyScene* ms, QObject* parent)
    : Monster(myPlayer, ms, parent)
{
    setSpeed(3);
    setHP(60);
    setDamage(15);
    setAttackCooldown(1200);
    setValueScore(250);
    idleSheet = new QPixmap(":/assets/doctor_idle.png");
    moveSheet = new QPixmap(":/assets/doctor_move.png");

    loadAnimations();

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &DoctorMonster::updateAnimationFrame);
    animationTimer->start(150);
}

