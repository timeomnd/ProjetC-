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

    // Positions centrées (si sprites pas centrés, tu peux ajuster avec boundingRect()/2)
    QPointF monsterCenter = this->sceneBoundingRect().center();
    QPointF playerCenter = player->sceneBoundingRect().center();

    qreal dx = playerCenter.x() - monsterCenter.x();
    qreal dy = playerCenter.y() - monsterCenter.y();

    qreal distance = std::sqrt(dx * dx + dy * dy);

    // Si assez proche, on ne bouge plus
    if (distance < 1.0) return;

    // Normalisation du vecteur direction
    qreal vx = dx / distance;
    qreal vy = dy / distance;

    // Déplacement
    moveBy(vx * speed, vy * speed);

    // Détermination de l'animation
    QVector<QPixmap*>* currentAnimation = nullptr;
    if (std::abs(dx) > std::abs(dy)) {
        currentAnimation = dx > 0 ? &animationRightMove : &animationLeftMove;
    } else {
        currentAnimation = dy > 0 ? &animationDownMove : &animationUpMove;
    }

    // Animation
    if (currentAnimation && !currentAnimation->isEmpty()) {
        currentFrameIndex = (currentFrameIndex + 1) % currentAnimation->size();
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
                    mainScene->getScoreManager()->addPoints(-(this->getDamage())*3); // on enlève des points au score si on se fait toucher
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
    canShootTimer = new QTimer(this);
    idleSheet = new QPixmap(":/assets/ghost_idle.png");
    moveSheet = new QPixmap(":/assets/ghost_move.png");
    loadAnimations();

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &GhostMonster::updateAnimationFrame);
    animationTimer->start(150); // 150 ms par frame
}
void GhostMonster::attack() {
    if (!player) return;
    if (this->scene()) {
        if (this->collidesWithItem(player)) {
            if (lastAttackTime.elapsed() >= attackCooldown) {
                int newHP = player->getHP() - damage;
                player->setHP(newHP);
                disableShoot();
                if (player->getCanShoot()) {
                }
                if (player->getHealthBar()) {
                    player->getHealthBar()->updateHP(newHP);
                }
                if (mainScene->getScoreManager()) {
                    mainScene->getScoreManager()->addPoints(-(this->getDamage())*3); // on enlève des points au score si on se fait toucher
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
void GhostMonster::disableShoot() {
    if (player->getCanShoot()) {
        canShootTimer->start(3500); // 3.5 secondes
        player->setCanShoot(false);
        connect(canShootTimer, &QTimer::timeout, this,&GhostMonster::resetShoot);
    }
    else {
        canShootTimer->stop();
        canShootTimer->start(3500);
    }
}
void GhostMonster::resetShoot() {
    if (player->getCanShoot() == false) {
        player->setCanShoot(true);
        canShootTimer->stop();
    }
}


BirdMonster::BirdMonster(Player* myPlayer, MyScene* ms, QObject* parent)
    : Monster(myPlayer, ms, parent)
{
    setSpeed(3);
    setHP(50);
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
void DoctorMonster::setIsAttacking(bool a) {
    isAttacking = a;
}
bool DoctorMonster::getIsAttacking() const {
    return isAttacking;
}

DoctorMonster::DoctorMonster(Player* myPlayer, MyScene* ms, QObject* parent)
    : Monster(myPlayer, ms, parent)
{
    setSpeed(3);
    setHP(80);
    setDamage(20);
    setAttackCooldown(2000);
    setValueScore(250);
    idleSheet = new QPixmap(":/assets/doctor_idle.png");
    moveSheet = new QPixmap(":/assets/doctor_move.png");

    loadAnimations();

    animationTimer = new QTimer(this);
    attackAnimationTimer = new QTimer(this);
    connect(attackAnimationTimer, &QTimer::timeout, this, &DoctorMonster::updateAttackAnimation);

}
void DoctorMonster::setIsMoving(bool m) {
    isMoving = m;
}
bool DoctorMonster::getIsMoving() const {
    return isMoving;
}
void DoctorMonster::updateAttackAnimation() {
    if (!currentAttackAnimation || currentAttackAnimation->isEmpty()) return;

    if (attackFrameIndex < currentAttackAnimation->size()) {
        setPixmap(*(*currentAttackAnimation)[attackFrameIndex]);
        attackFrameIndex++;
    } else {
        attackAnimationTimer->stop();
        isAttacking = false;

        // Retour à l'animation idle
        QPointF monsterCenter = this->sceneBoundingRect().center();
        QPointF playerCenter = player->sceneBoundingRect().center();

        qreal dx = playerCenter.x() - monsterCenter.x();
        qreal dy = playerCenter.y() - monsterCenter.y();

        QVector<QPixmap*>* currentIdle = nullptr;
        if (std::abs(dx) > std::abs(dy)) {
            currentIdle = dx > 0 ? &animationRightIdle : &animationLeftIdle;
        } else {
            currentIdle = dy > 0 ? &animationDownIdle : &animationUpIdle;
        }

        if (currentIdle && !currentIdle->isEmpty()) {
            currentFrameIndex = 0;
            setPixmap(*(*currentIdle)[currentFrameIndex]);
        }
    }
}

void DoctorMonster::attack() {
    if (lastAttackTime.elapsed() >= attackCooldown) {
        if (isAttacking) return;
        if (isMoving) return;
        isAttacking = true;
        attackFrameIndex = 0;

        QPointF monsterCenter = this->sceneBoundingRect().center();
        QPointF playerCenter = player->sceneBoundingRect().center();

        qreal dx = playerCenter.x() - monsterCenter.x();
        qreal dy = playerCenter.y() - monsterCenter.y();

        if (std::abs(dx) > std::abs(dy)) {
            currentAttackAnimation = dx > 0 ? &animationAttackRight : &animationAttackLeft;
        } else {
            currentAttackAnimation = dy > 0 ? &animationAttackDown : &animationAttackUp;
        }

        if (currentAttackAnimation && !currentAttackAnimation->isEmpty()) {
            attackAnimationTimer->start(100); // 100 ms entre chaque frame
        }
        QPointF startPos = this->sceneBoundingRect().center();
        QPointF targetPos = player->sceneBoundingRect().center();
        Fireball* fb = new Fireball(startPos, targetPos, this->scene(), this);

        lastAttackTime.restart(); // reset du cooldown
    }
}


void DoctorMonster::move() {
    if (isAttacking) return;
    if (!player) return;

    QPointF monsterCenter = this->sceneBoundingRect().center();
    QPointF playerCenter = player->sceneBoundingRect().center();

    qreal dx = playerCenter.x() - monsterCenter.x();
    qreal dy = playerCenter.y() - monsterCenter.y();

    qreal distance = std::sqrt(dx * dx + dy * dy);

    const qreal minDistance = 125.0; // Distance à partir de laquelle le monstre s'arrête

    if (distance > minDistance) {
        setIsMoving(true);
        // Normalisation du vecteur direction
        qreal vx = dx / distance;
        qreal vy = dy / distance;

        moveBy(vx * speed, vy * speed);

        // Animation de déplacement
        QVector<QPixmap*>* currentAnimation = nullptr;
        if (std::abs(dx) > std::abs(dy)) {
            currentAnimation = dx > 0 ? &animationRightMove : &animationLeftMove;
        } else {
            currentAnimation = dy > 0 ? &animationDownMove : &animationUpMove;
        }

        if (currentAnimation && !currentAnimation->isEmpty()) {
            currentFrameIndex = (currentFrameIndex + 1) % currentAnimation->size();
            setPixmap(*(*currentAnimation)[currentFrameIndex]);
        }
    } else {
        setIsMoving(false);

        // Le joueur est à portée → lancer l'attaque
        attack();
        // Si on ne peut pas attaquer (déjà en train de le faire), alors idle
        if (!isAttacking) {
            QVector<QPixmap*>* currentIdle = nullptr;
            if (std::abs(dx) > std::abs(dy)) {
                currentIdle = dx > 0 ? &animationRightIdle : &animationLeftIdle;
            } else {
                currentIdle = dy > 0 ? &animationDownIdle : &animationUpIdle;
            }

            if (currentIdle && !currentIdle->isEmpty()) {
                currentFrameIndex = (currentFrameIndex + 1) % currentIdle->size();
                setPixmap(*(*currentIdle)[currentFrameIndex]);
            }
        }
    }
}

DoctorMonster::~DoctorMonster() {
    auto clearPixmaps = [](QVector<QPixmap*>& vec) {
        for (QPixmap* pixmap : vec) {
            delete pixmap;
        }
        vec.clear();
    };
    clearPixmaps(animationAttackLeft);
    clearPixmaps(animationAttackRight);
    clearPixmaps(animationAttackUp);
    clearPixmaps(animationAttackDown);
    clearPixmaps(*currentAttackAnimation);
    if (attackSheet) delete attackSheet;
    if (attackAnimationTimer) delete attackAnimationTimer;
    if (attackSheet) delete attackSheet;
    if (animationTimer) delete animationTimer;
}
void DoctorMonster::loadAnimations() {
    if (idleSheet->isNull() || moveSheet->isNull()) {
        qWarning("Erreur : sprites introuvables !");
        return;
    }

    int frameWidth = idleSheet->width() / 4;
    int frameHeight = idleSheet->height() /4 ;

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
    // Chargement des sprites d'attaque
    attackSheet = new QPixmap(":/assets/doctor_attack.png");
    if (!attackSheet->isNull()) {
        int attackFrameWidth = attackSheet->width() / 6;
        int attackFrameHeight = attackSheet->height() / 4;

        for (int i = 0; i < 6; ++i) {
            animationAttackDown.append(new QPixmap(attackSheet->copy(i * attackFrameWidth, 0 * attackFrameHeight, attackFrameWidth, attackFrameHeight)));
            animationAttackLeft.append(new QPixmap(attackSheet->copy(i * attackFrameWidth, 1 * attackFrameHeight, attackFrameWidth, attackFrameHeight)));
            animationAttackRight.append(new QPixmap(attackSheet->copy(i * attackFrameWidth, 2 * attackFrameHeight, attackFrameWidth, attackFrameHeight)));
            animationAttackUp.append(new QPixmap(attackSheet->copy(i * attackFrameWidth, 3 * attackFrameHeight, attackFrameWidth, attackFrameHeight)));
        }
    } else {
        qWarning("Erreur : doctor_attack introuvable !");
    }


    setPixmap(*animationDownIdle[0]);
}
SlimeMonster::SlimeMonster(Player* myPlayer, MyScene* ms, QObject* parent)
    : Monster(myPlayer, ms, parent)
{
    setSpeed(2);
    setHP(40);
    setDamage(10);
    setAttackCooldown(1200);
    setValueScore(250);
    slowTimer = new QTimer(this);
    idleSheet = new QPixmap(":/assets/slime_idle.png");
    moveLeftSheet = new QPixmap(":/assets/slime_move_left.png");
    moveRightSheet = new QPixmap(":/assets/slime_move_right.png");
    this->loadAnimations();
    // Timer pour déclencher un saut toutes les 2 secondes
    jumpCooldownTimer = new QTimer(this);
    connect(jumpCooldownTimer, &QTimer::timeout, this, &SlimeMonster::jump);
    jumpCooldownTimer->start(2000); // toutes les 2 secondes

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &SlimeMonster::updateAnimationFrame);
    animationTimer->start(150);
}
void SlimeMonster::loadAnimations() {
    if (idleSheet->isNull() || moveLeftSheet->isNull() || moveRightSheet->isNull()) {
        qWarning("Erreur : sprites introuvables !");
        return;
    }

    int frameWidth = idleSheet->width() / 4;
    int frameHeight = idleSheet->height();

    for (int i = 0; i < 4; ++i) {
        animationIdle.append(new QPixmap(idleSheet->copy(i * frameWidth, 0, frameWidth, frameHeight)));
        animationRightMove.append(new QPixmap(moveRightSheet->copy(i * frameWidth, 0, frameWidth, frameHeight)));
        animationLeftMove.append(new QPixmap(moveLeftSheet->copy(i * frameWidth, 0, frameWidth, frameHeight)));
    }
    // Chargement des sprites de saut
    jumpSheet = new QPixmap(":/assets/slime_jump.png");
    if (!jumpSheet->isNull()) {
        int jumpFrameWidth = jumpSheet->width() / 4;
        int jumpFrameHeight = jumpSheet->height() / 2;

        for (int i = 0; i < 4; ++i) {
            animationJumpUp.append(new QPixmap(jumpSheet->copy(i * jumpFrameWidth, 0, jumpFrameWidth, jumpFrameHeight)));
            animationJumpDown.append(new QPixmap(jumpSheet->copy(i * jumpFrameWidth, jumpFrameHeight, jumpFrameWidth, jumpFrameHeight)));
        }
    } else {
        qWarning("Erreur : slime_jump introuvable !");
    }
    // Chargement des sprites d'attaque
    attackSheet = new QPixmap(":/assets/slime_attack.png");
    if (!attackSheet->isNull()) {
        int attackFrameWidth = attackSheet->width() / 4;
        int attackFrameHeight = attackSheet->height();

        for (int i = 0; i < 4; ++i) {
            animationAttack.append(new QPixmap(attackSheet->copy(i * attackFrameWidth, 0, attackFrameWidth, attackFrameHeight)));
        }
    } else {
        qWarning("Erreur : slime_attack introuvable !");
    }

    setPixmap(*animationIdle[0]);
}

void SlimeMonster::move() {
    if (!player) return;

    QPointF monsterCenter = this->sceneBoundingRect().center();
    QPointF playerCenter = player->sceneBoundingRect().center();

    qreal dx = playerCenter.x() - monsterCenter.x();
    qreal dy = playerCenter.y() - monsterCenter.y();

    qreal distance = std::sqrt(dx * dx + dy * dy);
    if (distance < 1.0) return;

    qreal vx = dx / distance;
    qreal vy = dy / distance;

    moveBy(vx * speed, vy * speed);

    if (!isJumping) {
        QVector<QPixmap*>* currentAnimation = nullptr;
        if (std::abs(dx) > std::abs(dy)) {
            currentAnimation = dx > 0 ? &animationRightMove : &animationLeftMove;
        } else {
            currentAnimation = dy > 0 ? &animationLeftMove : &animationRightMove;
        }

        if (currentAnimation && !currentAnimation->isEmpty()) {
            currentFrameIndex = (currentFrameIndex + 1) % currentAnimation->size();
            setPixmap(*(*currentAnimation)[currentFrameIndex]);
        }
    }
}
void SlimeMonster::jump() {
    if (isJumping || animationJumpUp.isEmpty() || animationJumpDown.isEmpty()) return;

    isJumping = true;
    jumpFrameIndex = 0;

    if (!jumpTimer) {
        jumpTimer = new QTimer(this);
        connect(jumpTimer, &QTimer::timeout, this, [=]() {
            QVector<QPixmap*>* currentJump = nullptr;

            // Phase montée (0 à 3), puis descente (4 à 7)
            if (jumpFrameIndex < 4) {
                currentJump = &animationJumpUp;
            } else if (jumpFrameIndex < 8) {
                currentJump = &animationJumpDown;
            }
            if (currentJump && !currentJump->isEmpty()) {
                int index = jumpFrameIndex % 4;
                setPixmap(*(*currentJump)[index]);
            }

            jumpFrameIndex++;

            if (jumpFrameIndex >= 8) {
                jumpTimer->stop();
                isJumping = false;
                jumpFrameIndex = 0;
            }
        });
    }

    jumpTimer->start(100); // 100ms par frame → saut total ~800ms
}
void SlimeMonster::attack() {
    if (!player) return;
    if (this->scene()) {
        if (this->collidesWithItem(player)) {
            if (lastAttackTime.elapsed() >= attackCooldown) {

                // === Animation d'attaque ===
                if (!animationAttack.isEmpty()) {
                    QTimer* attackAnimationTimer = new QTimer(this);
                    int* attackFrameIndex = new int(0); // index temporaire

                    connect(attackAnimationTimer, &QTimer::timeout, this, [=]() mutable {
                        if (*attackFrameIndex < animationAttack.size()) {
                            setPixmap(*animationAttack[*attackFrameIndex]);
                            (*attackFrameIndex)++;
                        } else {
                            attackAnimationTimer->stop();
                            attackAnimationTimer->deleteLater();
                            delete attackFrameIndex;
                        }
                    });

                    attackAnimationTimer->start(100); // 100 ms par frame (0.4s total)
                }

                // === Infliger des dégâts ===
                int newHP = player->getHP() - damage;
                player->setHP(newHP);
                if (player->getHealthBar()) {
                    player->getHealthBar()->updateHP(newHP);
                }
                if (mainScene->getScoreManager()) {
                    mainScene->getScoreManager()->addPoints(-(this->getDamage()) * 3);
                }

                slow(); // applique le ralentissement
                qDebug() << "Attaque ! HP joueur :" << player->getHP();

                // Jouer le son
                QSoundEffect* currentSound = hitSounds[currentHitSoundIndex];
                if (currentSound->status() == QSoundEffect::Ready) {
                    currentSound->stop();
                    currentSound->play();
                }
                currentHitSoundIndex = (currentHitSoundIndex + 1) % hitSounds.size();

                lastAttackTime.restart(); // reset du cooldown
            }
        }
    }
}
void SlimeMonster::slow() {
    if (player->getSpeed()!=player->getInitalSpeed()) {
        slowTimer->stop();
        slowTimer->start(3500);
        return;
    }
    else {
        player->setSpeed(player->getSpeed()*0.95);
        slowTimer->start(3500); // 3.5 secondes de ralentissement
        connect(slowTimer, &QTimer::timeout, this,&SlimeMonster::resetSpeed);
    }
}
void SlimeMonster::resetSpeed() {
    if (player->getSpeed()!=player->getInitalSpeed()) {
        player->setSpeed(player->getInitalSpeed());
        slowTimer->stop();
    }
}
QTimer* SlimeMonster::getSlowTimer() const {
    return slowTimer;
}
SlimeMonster::~SlimeMonster() {
    // Supprimer tous les QPixmap* dans chaque QVector et vider les vecteurs
    resetSpeed();
    auto clearPixmaps = [](QVector<QPixmap*>& vec) {
        for (QPixmap* pixmap : vec) {
            delete pixmap;
        }
        vec.clear();
    };
    clearPixmaps(animationLeftMove);
    clearPixmaps(animationRightMove);
    clearPixmaps(animationJumpUp);
    clearPixmaps(animationJumpDown);
    if (jumpTimer) delete jumpTimer;
    if (jumpCooldownTimer) delete jumpCooldownTimer;
    if (slowTimer) delete slowTimer;
    clearPixmaps(animationAttack);
    if (attackSheet) delete attackSheet;

}
Fireball::Fireball(QPointF startPos, QPointF targetPos, QGraphicsScene* scene, QObject* parent)
    : QObject(parent), currentFrameIndex(0) {

    for (int i = 1; i <= 8; ++i) {
        QPixmap* frame = new QPixmap(QString(":/assets/FB_%1.png").arg(i));
        if (frame->isNull()) {
            qDebug() << "[Fireball] Failed to load frame:" << i;
        }
        animationFrames.append(frame);
    }
    setPixmap(animationFrames[0]->scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setPos(startPos);
    scene->addItem(this);

    qreal dx = targetPos.x() - startPos.x();
    qreal dy = targetPos.y() - startPos.y();
    qreal length = qSqrt(dx*dx + dy*dy);
    velocity = QPointF(dx / length * speed, dy / length * speed);
    qDebug() << "[Fireball] Velocity:" << velocity;

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &Fireball::moveAndAnimate);
    animationTimer->start(50); // 20 FPS
}

Fireball::~Fireball() {
    for (QPixmap* pix : animationFrames) delete pix;
}

void Fireball::moveAndAnimate() {
    setPos(pos() + velocity);
    currentFrameIndex = (currentFrameIndex + 1) % animationFrames.size();
    setPixmap(*animationFrames[currentFrameIndex]);

    if (!scene()->sceneRect().contains(pos())) {
        scene()->removeItem(this);
        deleteLater();
    }
}

