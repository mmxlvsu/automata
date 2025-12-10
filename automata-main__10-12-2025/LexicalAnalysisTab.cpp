#include "LexicalAnalysisTab.h"
#include "DiagramBuilder.h"
#include <QFont>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QRegularExpression>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBrush>
#include <QPen>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QGraphicsView>

LexicalAnalysisTab::LexicalAnalysisTab(QWidget* parent)
    : QWidget(parent)
{
    // ================= USER CODE AREA =================
    int leftX = 20;
    int topY = 20;

    // Label
    userlabel = new QLabel("Write your code here:", this);
    userlabel->setFont(QFont("Poppins", 14, QFont::Bold));
    userlabel->move(leftX, topY);

    // Input box
    userinput = new QTextEdit(this);
    userinput->setGeometry(leftX, 50, 900, 225);
    userinput->setFont(QFont("Consolas", 12));

    // Run button
    int runY = 50 + 225 + 10;
    run = new QPushButton("Run", this);
    run->setFont(QFont("Poppins", 10, QFont::Bold));
    run->setStyleSheet("background-color: #16163F; color: white;");
    run->setGeometry(leftX + 900 - 70, runY, 70, 30);
    connect(run, &QPushButton::clicked, this, &LexicalAnalysisTab::runLexicalAnalysis);

    // ================= DFA AREA (IMAGE PLACEHOLDER) =================
    int dfaY = runY + 40;

    // Title label
    dfa = new QLabel("DFA Diagram", this);
    dfa->setFont(QFont("Poppins", 14, QFont::Bold));
    dfa->move(leftX, dfaY);

    // Create scene and view
    dfaScene = new QGraphicsScene(this);
    dfaView  = new QGraphicsView(dfaScene, this);

    // View size and position
    dfaView->setGeometry(leftX, dfaY + 40, 900, 300);
    dfaView->setStyleSheet("background-color: white; border: 1px solid #aaa;");

    // Enable scrollbars for panning/swiping
    dfaView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    dfaView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Optional: make dragging the scene with mouse
    dfaView->setDragMode(QGraphicsView::ScrollHandDrag);

    // 1. Build the diagram
    DiagramBuilder::buildExampleDiagram(dfaScene);

    // 2. Get the exact size of the diagram
    QRectF diagramBounds = dfaScene->itemsBoundingRect();

    // 3. Expand scene to contain everything
    dfaScene->setSceneRect(diagramBounds);

// 4. Do NOT scale to fit – allow scrolling
// dfaView->fitInView(diagramBounds, Qt::KeepAspectRatio); // remove this

// Optional: smooth rendering
dfaView->setRenderHint(QPainter::Antialiasing);




    // Optional placeholder
    // QGraphicsTextItem* placeholder = dfaScene->addText("Place DFA Image Here");
    // placeholder->setPos(350, 250);


    // ================= TOKEN TABLE (RIGHT SIDE) =================
    QVBoxLayout* rightLayout = new QVBoxLayout();
    tokenlabel = new QLabel("Token Table", this);
    tokenlabel->setFont(QFont("Poppins", 14, QFont::Bold));
    rightLayout->addWidget(tokenlabel);

    tokenizationtable = new QTableWidget(this);
    tokenizationtable->setColumnCount(4);
    tokenizationtable->setHorizontalHeaderLabels({"Token", "Type", "Line", "Column"});
    tokenizationtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tokenizationtable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    rightLayout->addWidget(tokenizationtable);

    QWidget* rightContainer = new QWidget(this);
    rightContainer->setLayout(rightLayout);
    rightContainer->setGeometry(950, 20, 900, 950);
}

// ==========================================================
// ================  RUN LEXICAL ANALYSIS  ==================
// ==========================================================

void LexicalAnalysisTab::runLexicalAnalysis()
{
    tokenizationtable->setRowCount(0);
    QString code = userinput->toPlainText();
    QStringList lines = code.split('\n');

    // ================= REGEX DEFINITIONS =================
    QRegularExpression numberRegex(R"(^[0-9]+(\.[0-9]+)?)");

    QRegularExpression identifierRegex(R"(^[a-zA-Z_][a-zA-Z0-9_]*)");

    QStringList keywords = {"if", "elif", "else", "for", "while", "def", "return"};

    QRegularExpression operatorRegex(
        R"(^(==|!=|<=|>=|\*\*|//|\+=|-=|\*=|/=|\+|-|\*|/|%|=|<|>))"
    );

    QRegularExpression delimiterRegex(R"(^[\{\}\(\)\[\]:])");

    // ======================================================

    int row = 0;

    for (int lineNum = 0; lineNum < lines.size(); ++lineNum) {
        QString line = lines[lineNum];
        int col = 0;

        while (!line.isEmpty()) {

            // Skip whitespace properly
            int oldLen = line.length();
            line = line.trimmed();
            col += (oldLen - line.length());
            if (line.isEmpty()) break;

            QRegularExpressionMatch match;

            // Numbers
            match = numberRegex.match(line);
            if (match.hasMatch()) {
                QString tok = match.captured();
                tokenizationtable->insertRow(row);
                tokenizationtable->setItem(row, 0, new QTableWidgetItem(tok));
                tokenizationtable->setItem(row, 1, new QTableWidgetItem("Number"));
                tokenizationtable->setItem(row, 2, new QTableWidgetItem(QString::number(lineNum + 1)));
                tokenizationtable->setItem(row, 3, new QTableWidgetItem(QString::number(col + 1)));

                line.remove(0, tok.length());
                col += tok.length();
                row++;
                continue;
            }

            // Delimiters
            match = delimiterRegex.match(line);
            if (match.hasMatch()) {
                QString tok = match.captured();
                tokenizationtable->insertRow(row);
                tokenizationtable->setItem(row, 0, new QTableWidgetItem(tok));
                tokenizationtable->setItem(row, 1, new QTableWidgetItem("Delimiter"));
                tokenizationtable->setItem(row, 2, new QTableWidgetItem(QString::number(lineNum + 1)));
                tokenizationtable->setItem(row, 3, new QTableWidgetItem(QString::number(col + 1)));

                line.remove(0, tok.length());
                col += tok.length();
                row++;
                continue;
            }

            // Operators (IMPORTANT: must be above identifiers!)
            match = operatorRegex.match(line);
            if (match.hasMatch()) {
                QString tok = match.captured();
                tokenizationtable->insertRow(row);
                tokenizationtable->setItem(row, 0, new QTableWidgetItem(tok));
                tokenizationtable->setItem(row, 1, new QTableWidgetItem("Operator"));
                tokenizationtable->setItem(row, 2, new QTableWidgetItem(QString::number(lineNum + 1)));
                tokenizationtable->setItem(row, 3, new QTableWidgetItem(QString::number(col + 1)));

                line.remove(0, tok.length());
                col += tok.length();
                row++;
                continue;
            }

            // Identifiers or Keywords
            match = identifierRegex.match(line);
            if (match.hasMatch()) {
                QString tok = match.captured();

                tokenizationtable->insertRow(row);
                tokenizationtable->setItem(row, 0, new QTableWidgetItem(tok));

                if (keywords.contains(tok))
                    tokenizationtable->setItem(row, 1, new QTableWidgetItem("Keyword"));
                else
                    tokenizationtable->setItem(row, 1, new QTableWidgetItem("Identifier"));

                tokenizationtable->setItem(row, 2, new QTableWidgetItem(QString::number(lineNum + 1)));
                tokenizationtable->setItem(row, 3, new QTableWidgetItem(QString::number(col + 1)));

                line.remove(0, tok.length());
                col += tok.length();
                row++;
                continue;
            }

            // Unknown Token
            tokenizationtable->insertRow(row);
            QString unknown = line.left(1);
            tokenizationtable->setItem(row, 0, new QTableWidgetItem(unknown));
            tokenizationtable->setItem(row, 1, new QTableWidgetItem("Unknown"));
            tokenizationtable->setItem(row, 2, new QTableWidgetItem(QString::number(lineNum + 1)));
            tokenizationtable->setItem(row, 3, new QTableWidgetItem(QString::number(col + 1)));

            line.remove(0, 1);
            col += 1;
            row++;
        }
    }

    // Emit tokens for other tabs
    QList<QStringList> tokensList;
    for (int r = 0; r < tokenizationtable->rowCount(); ++r) {
        QStringList t;
        t << tokenizationtable->item(r, 0)->text()
          << tokenizationtable->item(r, 1)->text()
          << tokenizationtable->item(r, 2)->text()
          << tokenizationtable->item(r, 3)->text();
        tokensList.append(t);
    }
    emit tokensReady(tokensList);
}
