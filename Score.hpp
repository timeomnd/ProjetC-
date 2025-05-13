//
// Created by timti on 10/05/2025.
//

#ifndef SCORE_H
#define SCORE_H
#include <QObject>
#include <QFile>
#include <QGraphicsTextItem>

class MyScene;
class ScoreManager : public QObject {
    Q_OBJECT
public :
    ScoreManager(QGraphicsScene* scene, QObject* parent = nullptr);
    int getCurrentScore() const;
    int getLastScore() const;
    int getBestScore() const;
    QGraphicsTextItem* getScoreText() const;

    void addPoints(int points);
    void resetCurrentScore();
    void saveScore(); //pour enregistrer le score a la fin de la partie
    ~ScoreManager();


private:
    int currentScore;
    int lastScore;
    int bestScore;
    QGraphicsScene* scene;
    QGraphicsTextItem* scoreText;

    void loadBestScore();
    void writeBestScore();
};
#endif //SCORE_H
