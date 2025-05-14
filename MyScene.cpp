#include "MyScene.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPointF targetPos = event->scenePos();
        player->shoot(targetPos);
    }
    QGraphicsScene::mousePressEvent(event);
    player->setFocus();
}

MyScene::MyScene(QGraphicsView* mainView, MainWindow* mw, QObject* parent)
    : QGraphicsScene(parent), mainWindow(mw), map(nullptr), scoreManager(nullptr),
      healthbarTimer(nullptr), spawnTimer(nullptr), player(nullptr), playerInitialized(false), scoreTimer(nullptr) {

    setBackgroundBrush(Qt::black);

    healthbarTimer = new QTimer(this);
    connect(healthbarTimer, &QTimer::timeout, [this]() {
        if (player && player->getHealthBar()) {
            auto view = views().first();
            QPointF pos = view->mapToScene(20, view->height() - 40);
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
        qDebug() << "ici";
        qDebug() << monster->scene();
        //if (monster->scene()) removeItem(monster);
        qDebug() << "la";
        monster->deleteLater();
    });
    setFocus();
}

void MyScene::initMap() {
    if (!map) {
        map = new Map(this, ":/assets/map.json");
    }
}

void MyScene::initPlayer() {
    if (!player) {
        player = new Player(mainWindow, this);
        player->setMainWindow(mainWindow);
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
            auto view = views().first();
            QPointF pos = view->mapToScene(20, view->height()-130);
            scoreManager->getScoreText()->setPos(pos);
        });
        scoreTimer->start(1);
    }
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
    int rand = QRandomGenerator::global()->bounded(1, 3);
    if (rand == 1) {
        monster = new SmallMonster(player, this);
    } else if (rand == 2) {
        monster = new BigMonster(player, this);
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
        delete map;
        map = nullptr;
    }

    if (scoreManager) {
        delete scoreManager;
        scoreManager = nullptr;
    }

    if (healthbarTimer) {
        healthbarTimer->stop();
        delete healthbarTimer;
        healthbarTimer = nullptr;
    }

    if (spawnTimer) {
        spawnTimer->stop();
        delete spawnTimer;
        spawnTimer = nullptr;
    }

    if (player) {
        if (player->getHealthBar()) {
            removeItem(player->getHealthBar());
        }
        removeItem(player);
        delete player;
        player = nullptr;
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

