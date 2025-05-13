#include "Score.hpp"

#include <QObject>
#include <qtextstream.h>
#include "MyScene.hpp"

ScoreManager::ScoreManager(QGraphicsScene* scene, QObject* parent)
    : QObject(parent), scene(scene), currentScore(0), lastScore(0), bestScore(0) {

    loadBestScore();

    // Création du texte HUD
    scoreText = scene->addText("Score: 0");
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setFont(QFont("Arial", 14));

}


int ScoreManager::getCurrentScore() const { return currentScore; }
int ScoreManager::getLastScore() const { return lastScore; }
int ScoreManager::getBestScore() const { return bestScore; }

void ScoreManager::addPoints(int points) {
    currentScore += points;

    if (scoreText) {
        scoreText->setPlainText(QString("Score: %1").arg(currentScore));
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
        in >> bestScore;
        file.close();
    }
}
void ScoreManager::writeBestScore() {
    QFile file("score.txt");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << bestScore;
        file.close();
    }
}
QGraphicsTextItem* ScoreManager::getScoreText() const {
    return scoreText;
}
ScoreManager::~ScoreManager() {
    // Rien à supprimer manuellement
    // La scène détruira scoreText automatiquement
}

