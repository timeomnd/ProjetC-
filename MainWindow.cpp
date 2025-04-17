#include "MainWindow.hpp"
#include "Player.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //création de la scène et d'une view
    this->mainView = new QGraphicsView;
    this->mainScene = new MyScene(this);
    this->mainView->setScene(mainScene);
    this->setCentralWidget(mainView);
    this->setWindowTitle("The Cursed ISEN");
    this->resize(1200, 800);
    QPalette palette;
    this->setPalette(QColorConstants::Svg::black);
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
    sound->stop();
    mainView = new QGraphicsView();
    mainScene = new MyScene(mainView, this);
    mainView->setScene(mainScene);
    setCentralWidget(mainView);
    // Assure que la vue transmet les événements clavier
    mainView->setFocusPolicy(Qt::StrongFocus);
    mainView->setFocus();
    launchGame = true; //permet d'éviter que le background se remette avec la fonction resize alors que la game est lancée
}
void MainWindow::resizeEvent(QResizeEvent *event){ //fonction déjà définie que on redéfini pour que ce soit dynamique

    QPixmap backgroundPixmap(":/assets/backgroundMenu.png");

    if (!backgroundPixmap.isNull() && !launchGame) {
        QSize viewSize = mainView->viewport()->size();
        QPixmap scaledBackground = backgroundPixmap.scaled(viewSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        QBrush brush(scaledBackground);
        brush.setStyle(Qt::TexturePattern); // Pour éviter la répétition

        mainScene->setSceneRect(0, 0, viewSize.width(), viewSize.height());
        mainScene->setBackgroundBrush(brush);
    }

    // Appelle la version d'origine de Qt pour que la fonction de base continue à faire les choses qu'on a pas codé
    QMainWindow::resizeEvent(event);
}