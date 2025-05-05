#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QVector>
#include <QRectF>


class Map : public QObject {
    Q_OBJECT
public:
    Map(QGraphicsScene* scene, const QString& jsonPath, QObject* parent = nullptr);

    const QVector<QRectF>& getCollisionRects() const;

private:
    void loadMapFromJson(const QString& jsonPath);
    QGraphicsScene* scene;
    QVector<QRectF> collisionRects;
};

#endif // MAP_H
