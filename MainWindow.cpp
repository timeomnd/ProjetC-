#include "MainWindow.hpp"
#include "Player.hpp"
#include "MyScene.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    mainScene(nullptr), gameOverScene(nullptr), mainView(nullptr), backgroundMenu(nullptr),
    gameOverBackground(nullptr), sound(nullptr), launchGame(false), scaledGameOverBackground(nullptr)
{
    //création de la scène et d'une view
    this->mainView = new NoScrollGraphicsView;
    this->mainScene = new MyScene(mainView,this);
    this->mainView->setScene(mainScene);
    this->setCentralWidget(mainView);
    this->setWindowTitle("The Cursed ISEN");
    this->resize(1200, 800);
    this->setPalette(QColorConstants::Svg::black);
    this->mainView->setDragMode(QGraphicsView::NoDrag);


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
    sound->setVolume(0.2);
    sound->setLoopCount(QSoundEffect::Infinite);
    sound->play();
}
void MainWindow::slot_restartGame(){
    QString ProgramPath = QCoreApplication::applicationFilePath(); //récupère le chemin du jeu
    QStringList arguments = QCoreApplication::arguments(); // récupère les arguments de la ligne de commande avec lesquels on lance

    QProcess::startDetached(ProgramPath,arguments); //relance le jeu
    QCoreApplication::exit(0); //ferme le jeu actuelle
}

QSoundEffect* MainWindow::getSound() const {
    return sound;
}


void MainWindow::slot_launchGame() {
    sound->stop();
    delete Play;
    Play = nullptr;
    if (mainScene) {
        mainScene->clear();
        mainScene->deleteLater();
        mainScene = nullptr;
    }
    if (gameOverScene) {
        gameOverScene->clear();
        gameOverScene->deleteLater();
        gameOverScene = nullptr;
    }
    if (mainView) {
        delete mainView;
        mainView = nullptr;
    }

    // Créer la nouvelle scène et vue
    mainView = new NoScrollGraphicsView(this);
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
    mainScene->initSound();


    QSizeF sceneSize = mainScene->sceneRect().size();
    QSize viewSize = this->size();


    double scaleFactorX = viewSize.width() / sceneSize.width();
    double scaleFactorY = viewSize.height() / sceneSize.height();

   
    double scaleFactor = qMax(scaleFactorX, scaleFactorY);


    mainView->resetTransform();
    mainView->scale(scaleFactor, scaleFactor);


    mainView->centerOn(mainScene->getPlayer());


    mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    mainView->setDragMode(QGraphicsView::NoDrag);
    mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
    if (!launchGame) {
        if (gameOverScene) {
            if (gameOverBackground) delete gameOverBackground;
            gameOverBackground = new QPixmap(":/assets/GameOverBackground.png");

            if (gameOverBackground && !gameOverBackground->isNull()) {
                QSize viewSize = mainView->viewport()->size();

                if (scaledGameOverBackground) delete scaledGameOverBackground;
                scaledGameOverBackground = new QPixmap(gameOverBackground->scaled(viewSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

                gameOverScene->clear();

                QGraphicsPixmapItem* backgroundItem = gameOverScene->addPixmap(*scaledGameOverBackground);
                backgroundItem->setZValue(-1);

                gameOverScene->setSceneRect(0, 0, viewSize.width(), viewSize.height());
            }
        } else {
            backgroundMenu = new QPixmap(":/assets/backgroundMenu.png");
            if (!backgroundMenu->isNull()) {
                QSize viewSize = mainView->viewport()->size();
                QPixmap scaledBackground = backgroundMenu->scaled(viewSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                QBrush brush(scaledBackground);
                brush.setStyle(Qt::TexturePattern);

                mainScene->setSceneRect(0, 0, viewSize.width(), viewSize.height());
                mainScene->setBackgroundBrush(brush);
            }
        }
    }
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);


    if (!launchGame) {
        updateBackground();
    }
}
NoScrollGraphicsView* MainWindow::getView() {
    return mainView;
}
MyScene*MainWindow::getScene() {
    return mainScene;
}
void MainWindow::adjustViewToScene() {
    if (!mainScene || !mainView) return;

    QSizeF sceneSize = mainScene->sceneRect().size();
    QSize viewSize = mainView->size(); 

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


    mainView->resetTransform();

    gameOverScene = new QGraphicsScene(this);
    mainView->setScene(gameOverScene);
    QSize viewSize = mainView->viewport()->size();
    gameOverScene->setSceneRect(0, 0, viewSize.width(), viewSize.height());
    mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainView->setDragMode(QGraphicsView::NoDrag);
    mainView->setFrameStyle(QFrame::NoFrame);
    mainView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainView->setRenderHint(QPainter::Antialiasing, true);

    launchGame = false; 
    updateBackground(); 

    QSoundEffect* gameOverSound = new QSoundEffect(this);
    gameOverSound->setSource(QUrl("qrc:/assets/gameOverSound.wav"));
    gameOverSound->setVolume(0.2);
    gameOverSound->play();

    if (mainView->layout()) {
        mainView->layout()->deleteLater();
    }

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
}





MainWindow::~MainWindow() {


    mainScene->deleteLater();
    mainScene = nullptr;
    mainView->deleteLater();
    mainView = nullptr;

    if (gameOverScene) {
        gameOverScene->deleteLater();
        gameOverScene = nullptr;
    }


}
bool MainWindow::getLaunchGame() {
    return launchGame;
}

