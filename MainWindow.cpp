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
    QPalette palette;
    this->setPalette(QColorConstants::Svg::black);
    QPixmap backgroundPixmap(":/assets/backgroundMenu.png");
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

}

MainWindow::~MainWindow(){

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



void MainWindow::slot_launchGame() {
    delete Play;
    Play = nullptr;

    if (mainScene) {
        mainScene->clear();
        delete mainScene;
        mainScene = nullptr;
    }

    if (gameOverSound) gameOverSound->stop();
    if (sound) sound->stop();

    // Créer la nouvelle scène et vue
    mainView = new QGraphicsView(this);
    mainView->setFixedSize(this->size()); // ou mieux :
    mainView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainView->setBackgroundBrush(Qt::black);
    mainView->setFocusPolicy(Qt::StrongFocus);

    mainScene = new MyScene(mainView, this);
    mainView->setScene(mainScene);
    mainView->scale(8.0, 8.0); // Zoom x8
    setCentralWidget(mainView);

    // Initialiser le jeu
    mainScene->initMap();
    mainScene->initPlayer();

    // Centrer la vue sur le joueur
    mainView->centerOn(mainScene->getPlayer());

    launchGame = true;
}



void MainWindow::resizeEvent(QResizeEvent *event){
    updateBackground();
    QMainWindow::resizeEvent(event);
}
void MainWindow::updateBackground() {
    QPixmap backgroundPixmap(":/assets/backgroundMenu.png");

    if (!backgroundPixmap.isNull() && !launchGame) {
        QSize viewSize = mainView->viewport()->size();
        QPixmap scaledBackground = backgroundPixmap.scaled(viewSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QBrush brush(scaledBackground);
        brush.setStyle(Qt::TexturePattern);

        mainScene->setSceneRect(0, 0, viewSize.width(), viewSize.height());
        mainScene->setBackgroundBrush(brush);
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