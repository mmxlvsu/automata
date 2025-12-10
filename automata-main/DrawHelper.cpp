#include "DrawHelper.h"
#include <QtMath>

QGraphicsEllipseItem* DrawHelper::createState(
    QGraphicsScene* scene, qreal x, qreal y, qreal size
){
    auto* state = scene->addEllipse(
        0, 0, size, size,
        QPen(Qt::black),
        QBrush(Qt::white)
    );
    state->setPos(x, y);
    state->setFlag(QGraphicsItem::ItemIsMovable);
    state->setFlag(QGraphicsItem::ItemIsSelectable);
    return state;
}

QGraphicsEllipseItem* DrawHelper::createFinalState(
    QGraphicsScene* scene, qreal x, qreal y, qreal size
){
    auto* outer = scene->addEllipse(x, y, size, size, QPen(Qt::black));
    scene->addEllipse(x + 10, y + 10, size - 20, size - 20, QPen(Qt::black));
    return outer; // return outer circle for connections
}


void DrawHelper::createArrow(
    QGraphicsScene* scene,
    QGraphicsEllipseItem* from,
    QGraphicsEllipseItem* to,
    const QString& label
){
    QPointF p1 = from->sceneBoundingRect().center();
    QPointF p2 = to->sceneBoundingRect().center();

    scene->addLine(QLineF(p1, p2), QPen(Qt::black, 2));

    QLineF line(p1, p2);
    double angle = qDegreesToRadians(-line.angle());

    qreal arrowSize = 12;
    QPointF arrowP1 = p2 + QPointF(cos(angle + 0.5) * -arrowSize,
                                   sin(angle + 0.5) * -arrowSize);
    QPointF arrowP2 = p2 + QPointF(cos(angle - 0.5) * -arrowSize,
                                   sin(angle - 0.5) * -arrowSize);

    QPolygonF head {p2, arrowP1, arrowP2};
    scene->addPolygon(head, QPen(Qt::black), QBrush(Qt::black));

    auto* text = scene->addText(label);
    text->setPos((p1.x()+p2.x())/2 - 10, (p1.y()+p2.y())/2 - 20);
}

void DrawHelper::createCurvedArrow(
    QGraphicsScene* scene,
    QGraphicsEllipseItem* from,
    QGraphicsEllipseItem* to,
    const QString& label,
    qreal curveOffset,
    qreal labelOffsetY
){
    QPointF p1 = from->sceneBoundingRect().center();
    QPointF p2 = to->sceneBoundingRect().center();

    QPointF mid = (p1 + p2) / 2;
    QPointF control(mid.x(), mid.y() - curveOffset);

    // Create path
    QPainterPath path(p1);
    path.quadTo(control, p2);

    auto* curvedLine = scene->addPath(path, QPen(Qt::black, 2));

    // Arrowhead at p2 (approx tangent)
    QLineF tangent(control, p2);
    double angle = qDegreesToRadians(-tangent.angle());

    qreal arrowSize = 12;
    QPointF arrowP1 = p2 + QPointF(cos(angle + 0.5) * -arrowSize,
                                   sin(angle + 0.5) * -arrowSize);
    QPointF arrowP2 = p2 + QPointF(cos(angle - 0.5) * -arrowSize,
                                   sin(angle - 0.5) * -arrowSize);

    QPolygonF head {p2, arrowP1, arrowP2};
    scene->addPolygon(head, QPen(Qt::black), QBrush(Qt::black));

    // Label
    auto* text = scene->addText(label);
    text->setPos(control.x() - 10, control.y() - 20);
}

void DrawHelper::createSelfLoop(
    QGraphicsScene* scene,
    QGraphicsEllipseItem* state,
    const QString& label
){
    QRectF r = state->sceneBoundingRect();
    QPointF topCenter(r.center().x(), r.top());

    // Create control points
    QPointF c1(topCenter.x() - 40, topCenter.y() - 60);
    QPointF c2(topCenter.x() + 40, topCenter.y() - 60);

    // Loop path
    QPainterPath path(topCenter);
    path.cubicTo(c1, c2, topCenter);

    scene->addPath(path, QPen(Qt::black, 2));

    // Arrowhead
    QPointF end = topCenter;
    QPointF left(end.x() - 10, end.y() - 5);
    QPointF right(end.x() + 10, end.y() - 5);

    QPolygonF head {end, left, right};
    scene->addPolygon(head, QPen(Qt::black), QBrush(Qt::black));

    // Label
    auto* text = scene->addText(label);
    text->setPos(topCenter.x() - 10, topCenter.y() - 80);
}

