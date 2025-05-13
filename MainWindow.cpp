#include "MainWindow.hpp"
#include "Player.hpp"
#include "MyScene.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //création de la scène et d'une view
    this->mainView = new QGraphicsView;
    this->mainScene = new MyScene(mainView,this);
    this->mainView->setScene(mainScene);
    this->setCentralWidget(mainView);
    this->setWindowTitle("The Cursed ISEN");
    this->resize(1200, 800);

    this->setPalette(QColorConstants::Svg::black);
    //bouton action
    helpMenu = menuBar()->addMenu(tr("&Help"));
    actionHelp = new QAction(tr("&About"), this);
    connect(actionHelp, SIGNAL(triggered()), this, SLOT(slot_aboutMenu()));
    helpMenu->addAction(actionHelp);

    //bouton restart
    restartGame = new QPushButton(tr("Restart"));
    QToolBar* toolbar = addToolBar(tr("Toolbar"));
    toolbar->addWidget(restartGame);
    connect(restartGame, &QPushButton::clicked, this, &MainWindow::slot_restartGame);

    //Création du bouton play
    mainLayout = new QVBoxLayout(mainView);
    mainView->setLayout(mainLayout);

    int id = QFontDatabase::addApplicationFont(":/assets/Creepster-Regular.ttf"); // récupère l'id du fichier font
    if (id == -1) {
        qDebug() << "La police n'est pas bien chargé";
    } else {
        QString family = QFontDatabase::applicationFontFamilies(id).at(0); //récupère la première famille retourner par la fonction applicationFontFamilies trouvé grâce a l'id

        // Et maintenant on utilise le nom de famille retourné
        QFont fontPlay(family, 20, QFont::Bold);
        Play = new QPushButton(tr("Enter the Cursed ISEN"));
        Play->setFont(fontPlay);
        Play->setFixedSize(800,100);
        QPalette palette;
        palette.setColor(QPalette::ButtonText, QColor("#8B0000"));
        Play->setPalette(palette);
        connect(Play, &QPushButton::clicked, this, &MainWindow::slot_launchGame);
        mainLayout->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(Play);
    }
    sound = new QSoundEffect(this);
    sound->setSource(QUrl("qrc:/assets/GameMusic.wav"));
    sound->setVolume(0.5);
    sound->setLoopCount(QSoundEffect::Infinite);
    sound->play();

    // création de la musique du game over
    gameOverSound = new QSoundEffect();
    gameOverSound->setSource(QUrl(":/assets/gameOverSound.mp3"));
    gameOverSound->setVolume(0.5);

}
void MainWindow::slot_restartGame(){
    QString ProgramPath = QCoreApplication::applicationFilePath(); //récupère le chemin du jeu
    QStringList arguments = QCoreApplication::arguments(); // récupère les arguments de la ligne de commande avec lesquels on lance

    QProcess::startDetached(ProgramPath,arguments); //relance le jeu
    QCoreApplication::exit(0); //ferme le jeu actuelle
}


void MainWindow::slot_aboutMenu(){
    QMessageBox msgBox;
    msgBox.setText("A small QT/C++ projet...");
    msgBox.setModal(true); // on souhaite que la fenetre soit modale i.e qu'on ne puisse plus cliquer ailleurs
    msgBox.exec();
}
QSoundEffect* MainWindow::getSound() const {
    return sound;
}

QSoundEffect* MainWindow::getGameOverSound() const {
    return gameOverSound;
}


void MainWindow::slot_launchGame() {
    sound->stop();
    delete Play;
    Play = nullptr;
    if (mainScene) {
        mainScene->clear();
        delete mainScene;
        mainScene = nullptr;
    }
    if (mainView) {
        delete mainView;
        mainView = nullptr;
    }

    // Créer la nouvelle scène et vue
    mainView = new QGraphicsView(this);
    mainScene = new MyScene(mainView, this);
    mainView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainView->setBackgroundBrush(Qt::black);
    mainView->setFocusPolicy(Qt::StrongFocus);
    mainView->setScene(mainScene);
    setCentralWidget(mainView);
    // Initialiser le jeu
    mainScene->initMap();
    mainScene->initPlayer();
    mainScene->initScoreManager();

    // Calculer la taille de la scène et de la vue
    QSizeF sceneSize = mainScene->sceneRect().size();
    QSize viewSize = this->size();

    // Calculer un facteur de zoom pour que la scène remplisse complètement la vue
    double scaleFactorX = viewSize.width() / sceneSize.width();
    double scaleFactorY = viewSize.height() / sceneSize.height();

    // Choisir le plus grand facteur de zoom pour que la scène remplisse toute la vue
    double scaleFactor = qMax(scaleFactorX, scaleFactorY);

    // Réinitialiser la transformation et appliquer le zoom
    mainView->resetTransform();
    mainView->scale(scaleFactor, scaleFactor);

    // Centrer la vue sur le joueur
    mainView->centerOn(mainScene->getPlayer());

    // Désactiver les barres de défilement
    mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Activer le mode de glissement pour naviguer dans la scène
    mainView->setDragMode(QGraphicsView::ScrollHandDrag);
    launchGame = true;
}

void MainWindow::resizeEvent(QResizeEvent *event){
    updateBackground();
    QMainWindow::resizeEvent(event);
    if (launchGame == true) {
        adjustViewToScene();
    }
}
void MainWindow::updateBackground() {
    qDebug() << "🪵 [DEBUG] updateBackground() appelé";
    QPixmap backgroundPixmap;

    if (!launchGame) {
        if (gameOverScene) {
            backgroundPixmap = QPixmap(":/assets/GameOverBackground.png");
        } else {
            backgroundPixmap = QPixmap(":/assets/backgroundMenu.png");
        }

        if (!backgroundPixmap.isNull()) {
            qDebug() << "✅ Background chargé avec succès";

            QSize viewSize = mainView->viewport()->size();
            QPixmap scaledBackground = backgroundPixmap.scaled(viewSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            QBrush brush(scaledBackground);
            brush.setStyle(Qt::TexturePattern);

            QGraphicsScene* currentScene = mainView->scene();
            if (currentScene) {
                currentScene->setSceneRect(0, 0, viewSize.width(), viewSize.height());
                currentScene->setBackgroundBrush(brush);
                qDebug() << "🎨 Background appliqué à la scène actuelle";
            } else {
                qDebug() << "⚠️ Aucune scène active pour appliquer le fond";
            }
        } else {
            qDebug() << "❌ Le QPixmap de fond est vide (non chargé)";
        }
    }
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event); // Laisse Qt gérer son affichage

    // On redessine le background correctement ici
    if (!launchGame) {
        updateBackground();
    }
}
QGraphicsView* MainWindow::getView() {
    return mainView;
}
MyScene*MainWindow::getScene() {
    return mainScene;
}
void MainWindow::adjustViewToScene() {
    if (!mainScene || !mainView) return;

    QSizeF sceneSize = mainScene->sceneRect().size();
    QSize viewSize = mainView->size(); // Utiliser mainView->size() ici, pas this->size()

    double scaleFactorX = viewSize.width() / sceneSize.width();
    double scaleFactorY = viewSize.height() / sceneSize.height();

    double scaleFactor = qMax(scaleFactorX, scaleFactorY);

    mainView->resetTransform();
    mainView->scale(scaleFactor, scaleFactor);

    mainView->centerOn(mainScene->getPlayer());
}
void MainWindow::die() {
    if (!mainScene) return;

    mainScene->setPlayerInitialized(false);

    if (sound) sound->stop();

    mainView->setScene(nullptr);
    mainScene->deleteLater();
    mainScene = nullptr;
    isGameOver = true;

    // Créer la scène de game over
    gameOverScene = new QGraphicsScene(this);
    mainView->setScene(gameOverScene); // ❗ utiliser la bonne scène

    // Nettoyage éventuel du layout précédent
    if (mainView->layout()) {
        mainView->layout()->deleteLater();
    }
    gameOverSound->play();
    Restart = new QPushButton(tr("Restart Game"));
    Restart->setFixedSize(400, 60);
    QFont myFont("Creepster", 20);
    QPalette palette;
    palette.setColor(QPalette::ButtonText, QColor("#8B0000"));
    Restart->setPalette(palette);
    Restart->setFont(myFont);
    connect(Restart, &QPushButton::clicked, this, &MainWindow::slot_launchGame);

    gameOverLayout = new QVBoxLayout(mainView);
    mainView->setLayout(gameOverLayout);
    gameOverLayout->addStretch();
    gameOverLayout->addWidget(Restart, 0, Qt::AlignCenter);
    gameOverLayout->addStretch();
    launchGame = false;
    // ❗ MAJ du fond
    updateBackground();
}



MainWindow::~MainWindow() {
    qDebug() << "🧹 Destruction de MainWindow";

    // Par sécurité, mais normalement déjà supprimés dans slot_launchGame()
    mainScene->deleteLater();
    mainScene = nullptr;
    mainView->deleteLater();
    mainView = nullptr;

    // Pas besoin de delete les QPushButton ou QSoundEffect avec parent !
}
bool MainWindow::getLaunchGame() {
    return launchGame;
}

