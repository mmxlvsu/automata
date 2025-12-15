#include "NFASimulatorTab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QPainterPath>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <cmath>
#include <QLabel>
#include <QFont>

NFASimulatorTab::NFASimulatorTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("NFA Simulator", this);
    titleLabel->setFont(QFont("Poppins", 16, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // 🔹 Pattern selector dropdown (now with 4 options)
    patternSelector = new QComboBox(this);
    patternSelector->setFont(QFont("Poppins", 12));
    patternSelector->addItems({
        "Identifier: [a-zA-Z_][a-zA-Z0-9_]*",
        "Integer: -?\\d+",
        "Float: -?\\d+\\.\\d+",
        "String: \"[^\"]*\""  
    });
    patternSelector->setStyleSheet("padding: 8px;");
    layout->addWidget(patternSelector);

    graphicsView = new QGraphicsView(this);
    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setScene(new QGraphicsScene(this));
    graphicsView->scene()->setBackgroundBrush(QColor("#636363"));
    layout->addWidget(graphicsView, 4);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    drawButton = new QPushButton("Draw NFA", this);
    drawButton->setFont(QFont("Poppins", 10, QFont::Bold));
    drawButton->setStyleSheet("background-color: #16163F; color: white; padding: 11px 32px;");
    btnLayout->addStretch();
    btnLayout->addWidget(drawButton);
    layout->addLayout(btnLayout);

    setLayout(layout);

    connect(patternSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NFASimulatorTab::onPatternChanged);
    connect(drawButton, &QPushButton::clicked, this, &NFASimulatorTab::drawNFA);

    drawNFA(); // Initial draw
}

void NFASimulatorTab::onPatternChanged(int index)
{
    currentPattern = static_cast<PatternType>(index);
    drawNFA();
}

void NFASimulatorTab::drawNFA()
{
    QGraphicsScene* scene = graphicsView->scene();
    scene->clear();

    QPen pen(Qt::black, 2);
    QBrush fillBrush(Qt::white);
    QBrush startBrush(Qt::lightGray);
    QBrush acceptBrush(QColor("#28a745")); // Green accept
    const QColor labelColor = Qt::darkBlue;

    auto drawState = [&](int x, int y, const QString& label, bool isStart, bool isAccept) {
        QGraphicsEllipseItem* circle = scene->addEllipse(x - 30, y - 30, 60, 60, pen,
                                                         isAccept ? acceptBrush : (isStart ? startBrush : fillBrush));
        QGraphicsTextItem* text = scene->addText(label);
        text->setFont(QFont("Arial", 12, QFont::Bold));
        text->setDefaultTextColor(Qt::black);
        text->setPos(x - text->boundingRect().width() / 2,
                     y - text->boundingRect().height() / 2);
        return circle;
    };

    auto drawTransition = [&](QGraphicsEllipseItem* from, QGraphicsEllipseItem* to,
                              const QString& label, bool isLoop = false) {
        QRectF fromRect = from->boundingRect();
        QRectF toRect = to->boundingRect();
        QPointF fromCenter = from->pos() + fromRect.center();
        QPointF toCenter = to->pos() + toRect.center();

        if (isLoop) {
            QPainterPath path;
            path.moveTo(fromCenter);
            path.cubicTo(fromCenter + QPointF(40, -50),
                         fromCenter + QPointF(90, -50),
                         fromCenter + QPointF(90, 0));
            path.lineTo(fromCenter + QPointF(90, 10));
            path.cubicTo(fromCenter + QPointF(90, 70),
                         fromCenter + QPointF(40, 70),
                         fromCenter);
            scene->addPath(path, pen);

            QGraphicsTextItem* t = scene->addText(label);
            t->setFont(QFont("Arial", 10));
            t->setDefaultTextColor(labelColor);
            t->setPos(fromCenter.x() + 30, fromCenter.y() - 65);
            return;
        }

        QLineF line(fromCenter, toCenter);
        double angle = line.angle();
        double rad = qDegreesToRadians(angle);

        int arrowSize = 10;
        QPointF p1 = toCenter - QPointF(arrowSize * cos(rad) - 5 * sin(rad),
                                        arrowSize * sin(rad) + 5 * cos(rad));
        QPointF p2 = toCenter - QPointF(arrowSize * cos(rad) + 5 * sin(rad),
                                        arrowSize * sin(rad) - 5 * cos(rad));

        scene->addLine(fromCenter.x(), fromCenter.y(),
                       toCenter.x(), toCenter.y(), pen);
        scene->addLine(toCenter.x(), toCenter.y(), p1.x(), p1.y(), pen);
        scene->addLine(toCenter.x(), toCenter.y(), p2.x(), p2.y(), pen);

        QPointF mid = (fromCenter + toCenter) / 2.0;
        QGraphicsTextItem* t = scene->addText(label);
        t->setFont(QFont("Arial", 10));
        t->setDefaultTextColor(labelColor);
        t->setPos(mid.x() - t->boundingRect().width() / 2,
                  mid.y() - t->boundingRect().height() / 2 - 15);
    };

    // === DRAW SELECTED NFA ===
    switch (currentPattern) {
    case Identifier: {
        int x0 = 120, y0 = 200;
        int x1 = 320, y1 = 200;
        int x2 = 520, y2 = 200;

        auto q0 = drawState(x0, y0, "q0", true, false);
        auto q1 = drawState(x1, y1, "q1", false, false);
        auto q2 = drawState(x2, y2, "q2", false, true);

        drawTransition(q0, q1, "[a-zA-Z_]");
        drawTransition(q1, q1, "[a-zA-Z0-9_]", true);
        drawTransition(q1, q2, "ε");

        QGraphicsTextItem* title = scene->addText("NFA for Identifiers");
        title->setFont(QFont("Poppins", 16, QFont::Bold));
        title->setDefaultTextColor(Qt::black);
        title->setPos(100, 80);
        break;
    }

    case Integer: {
        int x0 = 150, y0 = 200;
        int x1 = 300, y1 = 200;
        int x2 = 450, y2 = 200;

        auto q0 = drawState(x0, y0, "q0", true, false);
        auto q1 = drawState(x1, y1, "q1", false, false);
        auto q2 = drawState(x2, y2, "q2", false, true);

        drawTransition(q0, q1, "'-'");
        drawTransition(q0, q2, "[0-9]");
        drawTransition(q1, q2, "[0-9]");
        drawTransition(q2, q2, "[0-9]", true);

        QGraphicsTextItem* title = scene->addText("NFA for Integer Literals");
        title->setFont(QFont("Poppins", 16, QFont::Bold));
        title->setDefaultTextColor(Qt::black);
        title->setPos(100, 80);
        break;
    }

    case Float: {
        int x0 = 80,  y0 = 200;
        int x1 = 180, y1 = 200;
        int x2 = 280, y2 = 200;
        int x3 = 380, y3 = 200;
        int x4 = 480, y4 = 200;

        auto q0 = drawState(x0, y0, "q0", true, false);
        auto q1 = drawState(x1, y1, "q1", false, false);
        auto q2 = drawState(x2, y2, "q2", false, false);
        auto q3 = drawState(x3, y3, "q3", false, false);
        auto q4 = drawState(x4, y4, "q4", false, true);

        drawTransition(q0, q1, "'-'");
        drawTransition(q0, q2, "[0-9]");
        drawTransition(q1, q2, "[0-9]");
        drawTransition(q2, q2, "[0-9]", true);
        drawTransition(q2, q3, "'.'");
        drawTransition(q3, q4, "[0-9]");
        drawTransition(q4, q4, "[0-9]", true);

        QGraphicsTextItem* title = scene->addText("NFA for Float Literals");
        title->setFont(QFont("Poppins", 16, QFont::Bold));
        title->setDefaultTextColor(Qt::black);
        title->setPos(50, 80);
        break;
    }

    // 🔹 NEW: String Literal NFA
    case StringLiteral: {
        // q0 --'"'--> q1 --[^"]--> q1 (loop) --'"'--> q2 (accept)
        int x0 = 150, y0 = 200;
        int x1 = 320, y1 = 200;
        int x2 = 490, y2 = 200;

        auto q0 = drawState(x0, y0, "q0", true, false);
        auto q1 = drawState(x1, y1, "q1", false, false);
        auto q2 = drawState(x2, y2, "q2", false, true);

        drawTransition(q0, q1, "\"");              // Opening quote
        drawTransition(q1, q1, "[^\\\"]", true);   // Loop: any char except "
        drawTransition(q1, q2, "\"");              // Closing quote

        QGraphicsTextItem* title = scene->addText("NFA for String Literals");
        title->setFont(QFont("Poppins", 16, QFont::Bold));\
        title->setDefaultTextColor(Qt::black);
        title->setPos(100, 80);
        break;
    }
    }

    scene->setSceneRect(scene->itemsBoundingRect().adjusted(-50, -80, 50, 50));
    graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}