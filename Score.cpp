#include "Score.hpp"

#include <QObject>
#include <qtextstream.h>
#include "MyScene.hpp"

ScoreManager::ScoreManager(QGraphicsScene* scene, QObject* parent)
    : QObject(parent), scene(scene), currentScore(0), lastScore(0), bestScore(0){

    loadBestScore();

    scoreText = scene->addText(QString("Score: %1").arg(currentScore));
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setFont(QFont("Creepster", 6, QFont::Bold));

    lastScoreText = scene->addText(QString("Last Score: %1").arg(lastScore));
    lastScoreText->setDefaultTextColor(Qt::white);
    lastScoreText->setFont(QFont("Creepster", 6, QFont::Bold));

    bestScoreText = scene->addText(QString("Best Score: %1").arg(bestScore));
    bestScoreText->setDefaultTextColor(Qt::white);
    bestScoreText->setFont(QFont("Creepster", 6, QFont::Bold));
}


int ScoreManager::getCurrentScore() const { return currentScore; }
int ScoreManager::getLastScore() const { return lastScore; }
int ScoreManager::getBestScore() const { return bestScore; }

void ScoreManager::addPoints(int points) {
    if (currentScore + points < 0) {
        currentScore = 0;
    }
    else {
        currentScore += points;
    }
    if (scoreText) {
        scoreText->setPlainText(QString("Score: %1").arg(currentScore));
        if (bestScoreText && bestScore < currentScore) {
            bestScoreText->setPlainText(QString("Best Score: %1").arg(currentScore));
        }
    }
}

void ScoreManager::resetCurrentScore() {
    currentScore = 0;
}
void ScoreManager::saveScore() {
    lastScore = currentScore;
    if (currentScore > bestScore) {
        bestScore = currentScore;
        writeBestScore();
    }
    resetCurrentScore();
}
void ScoreManager::loadBestScore() {
    QFile file("score.txt");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        in >> bestScore >> lastScore;  // Lire bestScore et lastScore
        file.close();
    }
    qDebug() << "Best Score: " << bestScore;
    qDebug() << "Last Score: " << lastScore;
    qDebug() << "coucou";
}


void ScoreManager::writeBestScore() {
    QFile file("score.txt");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << bestScore << " " << lastScore;  // Sauvegarder les deux
        file.close();
    }
}


QGraphicsTextItem* ScoreManager::getScoreText() const {
    return scoreText;
}
QGraphicsTextItem* ScoreManager::getBestScoreText() const {
    return bestScoreText;
}
QGraphicsTextItem* ScoreManager::getLastScoreText() const {
    return lastScoreText;
}
ScoreManager::~ScoreManager() {}

