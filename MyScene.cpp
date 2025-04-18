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

MyScene::MyScene(QGraphicsView* mainView, QObject* parent): QGraphicsScene(parent), map(nullptr), player(nullptr) {
    
    setSceneRect(0, 0, 1200, 800); // Valeurs par défaut

    
    // Configuration du spawn de monstres
    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &MyScene::spawnMonster);
    spawnTimer->start(15000); // 15 secondes

    // Active les événements souris
    setFocus(); // Permet à la scène de recevoir les événements clavier/souris
}

void MyScene::initPlayer() {
    if (!player) {
        player = new Player();
        addItem(player);
        player->setPos(sceneRect().center());
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

    for (int i = 0; i < maxAttempts; ++i) {
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
        // Pas trouvé de position éloignée donc position de secours dans le coin de la map
        spawnPos = QPointF(0, 0);
    }
    int random = QRandomGenerator::global()->bounded(1, 3); // entier entre a inclus et b exclus
    if (random == 1) {
        BigMonster* monster = new BigMonster(player, this);
        monster->setPos(spawnPos);
        addItem(monster);
        activeMonsters.append(monster);
    }
    else if (random == 2) {
        SmallMonster* monster = new SmallMonster(player, this);
        monster->setPos(spawnPos);
        addItem(monster);
        activeMonsters.append(monster);
    }

}


MyScene::~MyScene() {
    qDeleteAll(activeMonsters); // Détruit tous les monstres
    delete map;
    if (spawnTimer){
        spawnTimer->stop(); // Arrête le timer
        delete spawnTimer;
    }
}