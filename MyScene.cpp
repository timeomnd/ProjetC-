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
    : QGraphicsScene(parent), mainWindow(mw) {

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
    spawnTimer->start(7500);

    // Connect pour gérer la destruction des monstres
    connect(this, &MyScene::monsterDestroyed, this, [this](Monster* monster) {
        activeMonsters.removeOne(monster); // Retirer de la liste
        removeItem(monster);              // Retirer de la scène
        delete monster;                   // Libérer la mémoire
    });

    setFocus();
}

void MyScene::initPlayer() {
    if (!player) {
        player = new Player();
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

        // Connecte le signal destroyed du monstre à une lambda
        connect(monster, &Monster::destroyed, this, [this](QObject* obj) {
            Monster* monster = qobject_cast<Monster*>(obj);
            if (monster) {
                emit monsterDestroyed(monster);
            }
        });
    }
}
void MainWindow::die() {
    if (mainScene) {
        mainScene->clear();
    }
    gameOverSound = new QSoundEffect(this);
    gameOverSound->setSource(QUrl("qrc:/assets/gameOverSound.wav"));
    gameOverSound->setVolume(0.7);
    gameOverSound->play();
    mainLayout = new QVBoxLayout(mainView);
    mainView->setLayout(mainLayout);

    int id = QFontDatabase::addApplicationFont(":/assets/Creepster-Regular.ttf"); // récupère l'id du fichier font
    if (id == -1) {
        qDebug() << "La police n'est pas bien chargé";
    } else {
        QString family = QFontDatabase::applicationFontFamilies(id).at(0); //récupère la première famille retourner par la fonction applicationFontFamilies trouvé grâce a l'id

        // Et maintenant on utilise le nom de famille retourné
        QFont fontPlay(family, 20, QFont::Bold);
        Play = new QPushButton(tr("restart Game"));
        Play->setFont(fontPlay);
        Play->setFixedSize(800,100);
        QPalette palette;
        palette.setColor(QPalette::ButtonText, QColor("#8B0000"));
        Play->setPalette(palette);
        connect(Play, &QPushButton::clicked, this, &MainWindow::slot_launchGame);
        mainLayout->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(Play);
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