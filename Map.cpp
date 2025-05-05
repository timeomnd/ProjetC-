#include "map.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGraphicsPixmapItem>
#include <QDebug>

Map::Map(QGraphicsScene* scene, const QString& jsonPath, QObject* parent)
    : QObject(parent), scene(scene) {
    loadMapFromJson(jsonPath);
}

const QVector<QRectF>& Map::getCollisionRects() const {
    return collisionRects;
}

void Map::loadMapFromJson(const QString& jsonPath) {
    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Impossible d'ouvrir la map :" << jsonPath;
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject map = doc.object();
    QJsonArray layers = map["layers"].toArray();
    int tileWidth = map["tilewidth"].toInt();
    int tileHeight = map["tileheight"].toInt();
    int widthInTiles = map["width"].toInt();

    // === Chargement du tileset ===
    QPixmap tileset(":/assets/Etage1.png");
    if (tileset.isNull()) {
        qWarning() << "Erreur de chargement du tileset.";
        return;
    }

    int columns = tileset.width() / tileWidth;
    int firstgid = 1; // généralement 1 si un seul tileset

    for (const QJsonValue& layerVal : layers) {
        QJsonObject layer = layerVal.toObject();
        QString type = layer["type"].toString();
        QString name = layer["name"].toString();

        if (type == "tilelayer") {
            QJsonArray data = layer["data"].toArray();
            for (int i = 0; i < data.size(); ++i) {
                int gid = data[i].toInt();
                if (gid == 0) continue;

                int x = (i % widthInTiles) * tileWidth;
                int y = (i / widthInTiles) * tileHeight;

                int id = gid - firstgid;
                int tileX = (id % columns) * tileWidth;
                int tileY = (id / columns) * tileHeight;
                QPixmap tileImg = tileset.copy(tileX, tileY, tileWidth, tileHeight);
                scene->addPixmap(tileImg)->setPos(x, y);
            }
        }

        // On ignore l'affichage des collisions ici
        else if (type == "objectgroup" && name == "COLLISIONS") {
            QJsonArray objects = layer["objects"].toArray();
            for (const QJsonValue& objVal : objects) {
                QJsonObject obj = objVal.toObject();
                QRectF rect(obj["x"].toDouble(), obj["y"].toDouble(),
                            obj["width"].toDouble(), obj["height"].toDouble());
                collisionRects.append(rect);
                // Pas d'affichage graphique ici
            }
        }
    }
}
