#include "MyScene.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

void MyScene::keyPressEvent(QKeyEvent* event) {

    if (event->key() == Qt::Key_Escape) {
        isPaused = !isPaused;
        
        // Gérer les timers
        if (isPaused) {
            if (healthbarTimer && healthbarTimer->isActive()) healthbarTimer->stop();
            if (spawnTimer && spawnTimer->isActive()) spawnTimer->stop();
            if (scoreTimer && scoreTimer->isActive()) scoreTimer->stop();

            if (player) player->pause(); // Arrêter le joueur

            for (Monster* monster : activeMonsters) {
                monster->pause();
            }
            
            // Ajouter un texte "PAUSE"
            if (!pauseText) {
                pauseText = new QGraphicsTextItem("PAUSE");
                pauseText->setDefaultTextColor(Qt::black);
                pauseText->setFont(QFont("Arial", 50, QFont::Bold));
                addItem(pauseText);
                pauseText->setPos(width()/2 - pauseText->boundingRect().width()/2, 
                                height()/2 - pauseText->boundingRect().height()/2);
            }
        } 
        else {
            if (healthbarTimer) healthbarTimer->start(1);
            if (spawnTimer) spawnTimer->start(5000);
            if (scoreTimer) scoreTimer->start(1);

            if (player) player->resume();
            
            // Supprimer le texte
            if (pauseText) {
                removeItem(pauseText);
                delete pauseText;
                pauseText = nullptr;
            }

            for (Monster* monster : activeMonsters) {
                monster->resume();
            }
        }
    }
    QGraphicsScene::keyPressEvent(event);
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (isPaused || !player || !player->isAlive()) return; 

    QPointF targetPos = event->scenePos();
    QPointF playerCenter = player->pos() + QPointF(player->boundingRect().width() / 2, 
                                                 player->boundingRect().height() / 2);

    if (event->button() == Qt::LeftButton) {
        if (player->getCanShoot()) {
            player->getCurrentWeapon()->shoot(playerCenter, targetPos);
        }
    }
    
    QGraphicsScene::mousePressEvent(event);
    player->setFocus();
}

MyScene::MyScene(NoScrollGraphicsView* mainView, MainWindow* mw, QObject* parent)
    : QGraphicsScene(parent), mainWindow(mw), map(nullptr), scoreManager(nullptr),
      healthbarTimer(nullptr), spawnTimer(nullptr), player(nullptr), playerInitialized(false), scoreTimer(nullptr) {
    setBackgroundBrush(Qt::black);

    healthbarTimer = new QTimer(this);
    connect(healthbarTimer, &QTimer::timeout, [this]() {
        if (player && player->getHealthBar()) {
            QPointF pos = mainWindow->getView()->mapToScene(20, mainWindow->getView()->height() - 40);
            player->getHealthBar()->setPos(pos);
        }
    });
    healthbarTimer->start(1);

    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &MyScene::spawnMonster);
    spawnTimer->start(5000);

    connect(this, &MyScene::monsterDestroyed, this, [this](Monster* monster) {
        if (!monster) return;
        scoreManager->addPoints(monster->getValueScore());
        activeMonsters.removeOne(monster);
        monster->deleteLater();
    });
    setFocus();

}

void MyScene::initMap() {
    if (!map) {
        map = new Map(this, ":/assets/map/map.json");
    }
}

void MyScene::initPlayer() {
    if (!player) {
        player = new Player(mainWindow, this, map);
        addItem(player);
        player->setPos(sceneRect().center());
        addItem(player->getHealthBar());
        playerInitialized = true;
    }
}

void MyScene::initScoreManager() {
    if (!scoreManager) {
        scoreTimer = new QTimer(this);
        scoreManager = new ScoreManager(this);
        connect(scoreTimer, &QTimer::timeout, [this]() {
            QPointF pos = mainWindow->getView()->mapToScene(10, mainWindow->getView()->height()-120);
            scoreManager->getScoreText()->setPos(pos);
            QPointF pos2 = mainWindow->getView()->mapToScene(10, mainWindow->getView()->height()-160);
            scoreManager->getBestScoreText()->setPos(pos2);
            QPointF pos3 = mainWindow->getView()->mapToScene(10, mainWindow->getView()->height()-200);
            scoreManager->getLastScoreText()->setPos(pos3);
        });
        scoreTimer->start(1);
    }
}
void MyScene::initSound() {
    sound = new QSoundEffect(this);
    sound->setSource(QUrl("qrc:/assets/InGameSound.wav"));
    sound->setVolume(0.1);
    sound->setLoopCount(QSoundEffect::Infinite);
    sound->play();
}
void MyScene::spawnMonster() {
    if (!player || !playerInitialized) return;

    QPointF playerPos = player->pos();
    QPointF spawnPos;
    const int minDistance = 100;
    const int maxAttempts = 50;
    bool valid = false;

    for (int i = 0; i < maxAttempts; ++i) {
        int x = QRandomGenerator::global()->bounded(0, int(width()));
        int y = QRandomGenerator::global()->bounded(0, int(height()));
        QPointF pos(x, y);
        if (QLineF(playerPos, pos).length() >= minDistance) {
            spawnPos = pos;
            valid = true;
            break;
        }
    }

    if (!valid) spawnPos = QPointF(0, 0);

    Monster* monster = nullptr;
    int rand = QRandomGenerator::global()->bounded(1, 5);
    if (rand == 1) {
        monster = new DoctorMonster(player, this);
    } else if (rand == 2) {
        monster = new BirdMonster(player, this);
    }
    else if (rand == 3) {
        monster = new GhostMonster(player, this);
    }
    else if (rand == 4) {
        monster = new SlimeMonster(player, this);
    }
    if (monster) {
        monster->setPos(spawnPos);
        addItem(monster);
        activeMonsters.append(monster);
    }
}
//  Méthode à appeler quand un monstre meurt
void MyScene::destroyMonster(Monster* monster) {
    if (!monster) return;
    emit monsterDestroyed(monster); // déclenche le slot qui s'occupe du reste
}

void MyScene::setPlayerInitialized(bool initialized) {
    playerInitialized = initialized;
}

MyScene::~MyScene() {
    qDeleteAll(activeMonsters);
    activeMonsters.clear();

    if (map) {
        map->deleteLater();
        map = nullptr;
    }

    if (scoreManager) {
        scoreManager->deleteLater();
        scoreManager = nullptr;
    }

    if (healthbarTimer) {
        healthbarTimer->stop();
        healthbarTimer->deleteLater();
        healthbarTimer = nullptr;
    }

    if (spawnTimer) {
        spawnTimer->stop();
        spawnTimer->deleteLater();
        spawnTimer = nullptr;
    }

    if (player) {
        if (player->getHealthBar()) {
            delete player->getHealthBar();
        };
        player->deleteLater();
        player = nullptr;
    }
    if (sound) {
        sound->stop();
        sound->deleteLater();
        sound = nullptr;
    }
}

// Getters et setters
QTimer* MyScene::getHealthbarTimer() const { return healthbarTimer; }
QTimer* MyScene::getSpawnTimer() const { return spawnTimer; }
QTimer* MyScene::getScoreTimer() const { return scoreTimer; }
QList<Monster*>& MyScene::getActiveMonsters() { return activeMonsters; }
ScoreManager* MyScene::getScoreManager() const { return scoreManager; }
void MyScene::setHealthbarTimer(QTimer* timer) { healthbarTimer = timer; }
void MyScene::setSpawnTimer(QTimer* timer) { spawnTimer = timer; }
Player* MyScene::getPlayer() { return player; }

