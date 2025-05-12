#ifndef MAP_HPP
#define MAP_HPP

#include <QObject>
#include <QGraphicsScene>
#include <QVector>
#include <QRectF>
#include <QString>

class Map : public QObject {
    Q_OBJECT
public:
    explicit Map(QGraphicsScene* scene, const QString& jsonPath, QObject* parent = nullptr);
    const QVector<QRectF>& getCollisionRects() const;
    Map::~Map();

private:
    QGraphicsScene* scene;
    QVector<QRectF> collisionRects;

    void loadMapFromJson(const QString& jsonPath);
};


#endif // MAP_HPP
