#include "MyScene.hpp"



MyScene::MyScene(QGraphicsView* mainView, QObject* parent) : QGraphicsScene(parent) {
    // Ajout du joueur à la scène
    player = new Player();
    this->addItem(player); // Ajoute le joueur à la scène
    player->setPos(mainView->width()/2, mainView->height()/2); // Position initiale
    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &MyScene::spawnMonster);
    spawnTimer->start(15000); // tout les 15 secondes un nouveau spawn
}
void MyScene::spawnMonster() {
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
    }
    else if (random == 2) {
        SmallMonster* monster = new SmallMonster(player, this);
        monster->setPos(spawnPos);
        addItem(monster);
    }

}

MyScene::MyScene(QObject* parent) : QGraphicsScene(parent) {
}
MyScene::~MyScene() {
    delete map;
}
