#include "MainWindow.hpp"
#include "Player.hpp"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    mainView = new QGraphicsView(this);
    mainView->setFixedSize(1200, 800); 

    this->mainScene = new MyScene(mainView, this); // Passer la vue à la scène
    this->mainView->setScene(mainScene);
    this->setCentralWidget(mainView);
    this->setWindowTitle("The Cursed ISEN");
    
    mainView->setBackgroundBrush(Qt::black);
    QPixmap backgroundPixmap(":/assets/backgroundMenu.png");

    

    if (!backgroundPixmap.isNull() && !launchGame) {
        QSize viewSize = mainView->viewport()->size();
        QPixmap scaledBackground = backgroundPixmap.scaled(viewSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        QBrush brush(scaledBackground);
        brush.setStyle(Qt::TexturePattern); // Pour éviter la répétition

        mainScene->setSceneRect(0, 0, viewSize.width(), viewSize.height());
        mainScene->setBackgroundBrush(brush);
    }

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
        palette.setColor(QPalette::ButtonText, QColor("#8B0000")); // Changer la couleur du texte en orange
        Play->setPalette(palette);
        connect(Play, &QPushButton::clicked, this, &MainWindow::slot_launchGame);
        mainLayout->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(Play);

    }


    media = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);

    audioOutput->setVolume(0.2);
    media->setAudioOutput(audioOutput);
    media->setSource(QUrl("qrc:/assets/GameMusic.wav"));

    media->setLoops(QMediaPlayer::Infinite);
    media->play();

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
    media->stop();

    // Recréer la vue avec une taille fixe
    mainView = new QGraphicsView(this);
    mainView->setFixedSize(1200, 800); // Taille fixe
    mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Désactiver les barres de défilement
    mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mainScene = new MyScene(mainView, this);
    mainView->setScene(mainScene);
    mainView->setBackgroundBrush(Qt::black);
    mainView->setFocusPolicy(Qt::StrongFocus);
    setCentralWidget(mainView); // Mettre à jour le widget central

    // Initialiser le joueur explicitement
    mainScene->initPlayer();

    launchGame = true;
}


void MainWindow::resizeEvent(QResizeEvent *event) {
    if (!launchGame) { 
        // Applique l'image de fond uniquement pour le menu
        QPixmap backgroundPixmap(":/assets/backgroundMenu.png");
        if (!backgroundPixmap.isNull()) {
            QSize viewSize = mainView->viewport()->size();
            QPixmap scaledBackground = backgroundPixmap.scaled(viewSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            mainScene->setBackgroundBrush(scaledBackground);
        }
    }
    QMainWindow::resizeEvent(event);
}