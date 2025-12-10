#ifndef DRAWHELPER_H
#define DRAWHELPER_H

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>

class DrawHelper {
public:
    static QGraphicsEllipseItem* createState(
        QGraphicsScene* scene, qreal x, qreal y, qreal size = 30
    );

    // Old: static void createFinalState(...);
    static QGraphicsEllipseItem* createFinalState(
        QGraphicsScene* scene, qreal x, qreal y, qreal size = 30
    );


    // Straight arrow
    static void createArrow(
        QGraphicsScene* scene,
        QGraphicsEllipseItem* from,
        QGraphicsEllipseItem* to,
        const QString& label
    );

    // Curved arrow (Bezier)
    static void createCurvedArrow(
        QGraphicsScene* scene,
        QGraphicsEllipseItem* from,
        QGraphicsEllipseItem* to,
        const QString& label,
        qreal curveOffset = 70,
        qreal labelOffsetY = 2000
    );

    // Self-loop
    static void createSelfLoop(
        QGraphicsScene* scene,
        QGraphicsEllipseItem* state,
        const QString& label
    );
};

#endif
