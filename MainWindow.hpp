
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
class MainWindow : public QMainWindow {
    Q_OBJECT

private :
    MyScene* mainScene;
    QGraphicsView* mainView;
    QMenu* helpMenu;
    QPushButton* restartGame;
    QToolBar* toolbar;
    QPushButton* Play;
    QVBoxLayout* mainLayout;
protected:
    void resizeEvent(QResizeEvent *event) override;
public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

    public slots:
        void slot_aboutMenu();
        void slot_restartGame();
    private slots:
        void slot_launchGame();
};


#endif //MAINWINDOW_H
