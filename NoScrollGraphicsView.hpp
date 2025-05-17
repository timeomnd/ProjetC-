#ifndef NOSCROLLGRAPHICSVIEW_HPP
#define NOSCROLLGRAPHICSVIEW_HPP

#include <QGraphicsView>
#include <QWheelEvent>

class NoScrollGraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    explicit NoScrollGraphicsView(QWidget* parent = nullptr) : QGraphicsView(parent) {}

protected:
    void wheelEvent(QWheelEvent* event) override {
        // On bloque la molette
        event->ignore();
    }
};

#endif // NOSCROLLGRAPHICSVIEW_HPP
