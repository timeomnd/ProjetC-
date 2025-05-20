#include "Map.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QPixmap>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

Map::Map(QGraphicsScene* scene, const QString& jsonPath, QObject* parent)
    : QObject(parent), scene(scene) {
    loadMapFromJson(jsonPath);
}

const QVector<QRectF>& Map::getCollisionRects() const {
    return collisionRects;
}

void Map::loadMapFromJson(const QString& jsonPath) {
    qDebug() << "🔍 Tentative de chargement du fichier JSON:" << jsonPath;

    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "❌ Impossible d'ouvrir la map :" << jsonPath;
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    QJsonObject map = doc.object();

    int tileWidth = map["tilewidth"].toInt();
    int tileHeight = map["tileheight"].toInt();
    int width = map["width"].toInt();
    int height = map["height"].toInt();

    qDebug() << "📐 Dimensions de la carte:" << width << "x" << height;
    qDebug() << "📏 Dimensions des tuiles:" << tileWidth << "x" << tileHeight;

    // --- Préparer le chemin de base pour les images ---
    QString basePath = QFileInfo(jsonPath).absolutePath();

    // --- Charger tous les tilesets ---
    struct Tileset {
        int firstgid;
        QPixmap image;
        int columns;
    };
    QVector<Tileset> tilesets;

    QJsonArray tilesetArray = map["tilesets"].toArray();
    for (const QJsonValue& val : tilesetArray) {
        QJsonObject ts = val.toObject();

        if (!ts.contains("image")) {
            qDebug() << "⏭️ Tileset ignoré (chargé via .tsx externe):" << ts["source"].toString();
            continue;
        }

        Tileset tileset;
        tileset.firstgid = ts["firstgid"].toInt();
        tileset.columns = ts["columns"].toInt();
        QString imgPath = ts["image"].toString();

        QString fullPath = QDir(basePath).filePath(imgPath);  // Correction ici
        qDebug() << "🧩 Chargement du tileset:" << fullPath;

        if (!tileset.image.load(fullPath)) {
            qWarning() << "⚠️ Erreur chargement tileset:" << fullPath;
        } else {
            qDebug() << "✅ Tileset chargé avec succès:" << fullPath;
        }

        tilesets.append(tileset);

    }

    // --- Affichage des calques tilelayer ---
    QJsonArray layers = map["layers"].toArray();
    for (const QJsonValue& val : layers) {
        QJsonObject layer = val.toObject();
        QString layerName = layer["name"].toString();
        if (layer["type"].toString() != "tilelayer") continue;

        qDebug() << "🗺️ Affichage du calque:" << layerName;

        QJsonArray data = layer["data"].toArray();
        qDebug() << "📦 Nombre de tuiles dans le calque:" << data.size();

        for (int i = 0; i < data.size(); ++i) {
            int gid = data[i].toInt();
            if (gid == 0) continue;

            const Tileset* currentTileset = nullptr;
            for (int j = tilesets.size() - 1; j >= 0; --j) {
                if (gid >= tilesets[j].firstgid) {
                    currentTileset = &tilesets[j];
                    break;
                }
            }

            if (!currentTileset) {
                qWarning() << "❓ Aucun tileset trouvé pour GID:" << gid;
                continue;
            }

            if (currentTileset->image.isNull()) {
                qWarning() << "⚠️ Tileset avec image invalide pour GID:" << gid;
                continue;
            }

            int localId = gid - currentTileset->firstgid;
            int cols = currentTileset->columns;
            int sx = (localId % cols) * tileWidth;
            int sy = (localId / cols) * tileHeight;

            QPixmap tile = currentTileset->image.copy(sx, sy, tileWidth, tileHeight);

            int x = (i % width) * tileWidth;
            int y = (i / width) * tileHeight;

            QGraphicsPixmapItem* item = scene->addPixmap(tile);
            item->setPos(x, y);
        }
    }

    for (const QJsonValue& val : layers) {
        QJsonObject layer = val.toObject();
        
        if (layer["type"].toString() == "objectgroup" && layer["name"].toString() == "coli") {
            qDebug() << "🛑 Chargement des collisions";
            QJsonArray objects = layer["objects"].toArray();
            
            for (const QJsonValue& objVal : objects) {
                QJsonObject obj = objVal.toObject();
                QRectF rect(
                    obj["x"].toDouble(),
                    obj["y"].toDouble(),
                    obj["width"].toDouble(),
                    obj["height"].toDouble()
                );
                collisionRects.append(rect);
                
                // Optionnel : Visualisation des collisions
                auto collider = new QGraphicsRectItem(rect);
                collider->setPen(QPen(Qt::red));
                collider->setOpacity(0.3);
                scene->addItem(collider);
            }
        }
    }
    
    qDebug() << "✅ Chargement de la map terminé.";
    scene->setSceneRect(0, 0, width * tileWidth, height * tileHeight);
}

