#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //création de la scène et d'une view
    this->mainScene = new MyScene;
    this->mainView = new QGraphicsView;
    this->mainView->setScene(mainScene);
    this->setCentralWidget(mainView);
    this->setWindowTitle("Nom du jeu");
    this->resize(400, 800);
    QPixmap backgroundPixmap(":/assets/backgroundMenu.png");

    if (!backgroundPixmap.isNull()) {
        QSize viewSize = mainView->viewport()->size();


        QPixmap stretchedBackground = backgroundPixmap.scaled(
            viewSize,
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation
        );


        QBrush brush(stretchedBackground);
        brush.setStyle(Qt::SolidPattern);


        mainScene->setSceneRect(0, 0, viewSize.width(), viewSize.height());
        mainScene->setBackgroundBrush(brush);
    } else {
        qDebug() << "Erreur : image de fond introuvable.";
    }

    //bouton action
    helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction* actionHelp = new QAction(tr("&About"), this);
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
        qDebug() << "Échec du chargement de la police";
    } else {
        QString family = QFontDatabase::applicationFontFamilies(id).at(0); //récupère la première famille retourner par la fonction applicationFontFamilies trouvé grâce a l'id

        // Et maintenant on utilise le nom de famille retourné
        QFont fontPlay(family, 20, QFont::Bold);
        Play = new QPushButton(tr("Play"));
        Play->setFont(fontPlay);
        Play->setFixedSize(800,100);
        connect(Play, &QPushButton::clicked, this, &MainWindow::slot_launchGame);
        mainLayout->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(Play);

    }
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
    mainScene = new MyScene(this);
    mainView = new QGraphicsView(mainScene);
    setCentralWidget(mainView);
}
