
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QProcess>
#include <QCoreApplication>
#include "MyScene.hpp"
#include <QPushButton>
#include <QToolBar>
#include <QVBoxLayout>
#include <QFont>
#include <QFontDatabase>
#include <QDebug>
#include <QFile>
#include <QPalette>
#include <QPixmap>
#include <iostream>
#include <vector>
#include <QSoundEffect>
#include <QResizeEvent>  // Pour resizeEvent
#include <QSoundEffect>




class MainWindow : public QMainWindow {
    Q_OBJECT

private :
    MyScene* mainScene;
    QGraphicsView* mainView;
    QMenu* helpMenu;
    QAction* actionHelp;
    QPushButton* restartGame;
    QToolBar* toolbar;
    QPushButton* Play;
    QVBoxLayout* mainLayout;
    QSoundEffect* sound;
protected:

    void resizeEvent(QResizeEvent *event) override;
    bool launchGame;
public:

    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();
    void showEvent(QShowEvent *event);
    void updateBackground();
    public slots:
        void slot_aboutMenu();
        void slot_restartGame();
    private slots:
        void slot_launchGame();
};


#endif //MAINWINDOW_H
