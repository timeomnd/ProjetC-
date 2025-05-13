#include "MyScene.hpp"
#include <QGraphicsSceneMouseEvent>

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPointF targetPos = event->scenePos();
        player->shoot(targetPos);
    }
    QGraphicsScene::mousePressEvent(event); // Traitement de base
    player->setFocus(); // Rétablir le focus sur le joueur
}
MyScene::MyScene(QGraphicsView* mainView, MainWindow* mw, QObject* parent)
    : QGraphicsScene(parent), mainWindow(mw), map(nullptr){

    setBackgroundBrush(Qt::black);
    // Timer pour la barre de vie
    healthbarTimer = new QTimer(this);
    connect(healthbarTimer, &QTimer::timeout, [this]() {
        if (player && player->getHealthBar()) {
            auto view = views().first();
            QPointF pos = view->mapToScene(20, view->height() - 40);
            player->getHealthBar()->setPos(pos);
        }
    });
    healthbarTimer->start(16);

    // Timer pour le spawn des monstres
    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &MyScene::spawnMonster);
    spawnTimer->start(5000);
    // Connect pour gérer la destruction des monstres
    connect(this, &MyScene::monsterDestroyed, this, [this](Monster* monster) {
        scoreManager->addPoints(monster->getValueScore()); // getValueScore() retourne les points que le monstre vaut
        activeMonsters.removeOne(monster); // Retirer de la liste des monstres le monstre qui vient d'être tué
        removeItem(monster);              // Retirer de la scène
        monster->deleteLater();           // Libérer la mémoire
    });
    setFocus();
}
void MyScene::initMap() {
    if (!map) {
        map = new Map(this, ":/assets/map.json"); // adapte le chemin selon ton projet
    }
}
Player* MyScene::getPlayer() {
    return player;
}
void MyScene::initScoreManager() {
    if (!scoreManager) {
        scoreManager = new ScoreManager(this);
        auto view = views().first();
        QPointF pos = view->mapToScene(20, view->height() - 70);
        scoreManager->getScoreText()->setPos(pos);
    }
}
void MyScene::initPlayer() {
    if (!player) {
        player = new Player(mainWindow,this);
        addItem(player);
        player->setMainWindow(mainWindow);
        player->setPos(sceneRect().center());
        addItem(player->getHealthBar()); // Ajouter la barre de vie
        playerInitialized = true;
    }
}

void MyScene::setPlayerInitialized(bool initialized) {
    playerInitialized = initialized;
}

void MyScene::spawnMonster() {
    if (!player || !playerInitialized){
        return;
    }

    QPointF playerPos = player->pos();
    QPointF spawnPos;
    const int minDistance = 100;
    const int maxAttempts = 50;
    bool valid = false;

    for (int i = 0; i < maxAttempts; i++) {
        int x = QRandomGenerator::global()->bounded(0, int(width()));
        int y = QRandomGenerator::global()->bounded(0, int(height()));
        QPointF goodPosition(x, y);

        double distance = QLineF(playerPos, goodPosition).length();

        if (distance >= minDistance) {
            spawnPos = goodPosition;
            valid = true;
            break; // bonne position trouvée
        }
    }

    if (!valid) {
        spawnPos = QPointF(0, 0);
    }
    int random = QRandomGenerator::global()->bounded(1, 3); // entier entre a inclus et b exclus
    Monster* monster = nullptr;

    if (random == 1) {
        monster = new BigMonster(player, this);
    } else if (random == 2) {
        monster = new SmallMonster(player, this);
    }
    if (monster) {
    monster->setPos(spawnPos);
    addItem(monster);
    activeMonsters.append(monster);

    // Connecte le signal destroyed du monstre à une fonction lambda
    connect(monster, &Monster::destroyed, this, [this](QObject* obj) {
        Monster* monster = qobject_cast<Monster*>(obj);
        if (monster) {
            emit monsterDestroyed(monster);
        }
    });
}
}
MyScene::~MyScene() {
    // Supprimer tous les monstres
    qDeleteAll(activeMonsters);
    activeMonsters.clear();

    // Supprimer la map
    delete map;
    map = nullptr;

    // Supprimer le score manager
    delete scoreManager;
    scoreManager = nullptr;

    // Stopper et supprimer les timers
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

    // Supprimer le joueur et sa healthbar
    if (player) {
        if (player->getHealthBar()) {
            removeItem(player->getHealthBar());
            // Pas besoin de delete healthBar manuellement car il est parenté à Player
        }

        removeItem(player);
        delete player;  // ✅ on fait un vrai delete ici
        player = nullptr;
    }
}

QTimer* MyScene::getHealthbarTimer() const {
    return healthbarTimer;
}

QTimer* MyScene::getSpawnTimer() const {
    return spawnTimer;
}

QList<Monster*>& MyScene::getActiveMonsters() {
    return activeMonsters;
}

ScoreManager* MyScene::getScoreManager() const {
    return scoreManager;
}

void MyScene::setHealthbarTimer(QTimer* timer) {
    healthbarTimer = timer;
}

void MyScene::setSpawnTimer(QTimer* timer) {
    spawnTimer = timer;
}

