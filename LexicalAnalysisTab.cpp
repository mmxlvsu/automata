#include "LexicalAnalysisTab.h"
#include <QFont>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QRegularExpression>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>

LexicalAnalysisTab::LexicalAnalysisTab(QWidget* parent)
    : QWidget(parent)
{
    // ================= USER CODE AREA =================
    int leftX = 20;
    int topY = 20;

    userlabel = new QLabel("Write your code here:", this);
    userlabel->setFont(QFont("Poppins", 14, QFont::Bold));
    userlabel->move(leftX, topY);

    userinput = new QTextEdit(this);
    userinput->setGeometry(leftX, 50, 900, 225);
    userinput->setFont(QFont("Consolas", 12));

    // Run button aligned to the right edge of the text area
    int runY = 50 + 225 + 10;
    int runWidth = 70;
    int runHeight = 30;
    run = new QPushButton("Run", this);
    run->setFont(QFont("Poppins", 10, QFont::Bold));
    run->setStyleSheet("background-color: #16163F; color: white;");
    run->setGeometry(leftX + 900 - runWidth, runY, runWidth, runHeight);
    connect(run, &QPushButton::clicked, this, &LexicalAnalysisTab::runLexicalAnalysis);

    // ================= DFA AREA =================
    int dfaY = runY + runHeight + 10;
    dfa = new QLabel("DFA Diagram", this);
    dfa->setFont(QFont("Poppins", 14, QFont::Bold));
    dfa->move(leftX, dfaY);

    int dfaPlaceholderY = dfaY + 40;
    int dfaPlaceholderWidth = 900;
    int dfaPlaceholderHeight = 600;
    dfaPlaceholder = new QLabel("DFA Diagram Placeholder", this);
    dfaPlaceholder->setGeometry(leftX, dfaPlaceholderY, dfaPlaceholderWidth, dfaPlaceholderHeight);
    dfaPlaceholder->setStyleSheet("background-color: #ddd; border: 1px solid #aaa;");

    // ================= Navigation buttons BELOW DFA placeholder =================
    int navWidth = 70;
    int navHeight = 30;
    int verticalGap = 10;   // space below DFA placeholder
    int horizontalGap = 10; // space between Prev and Next
    int navY = dfaPlaceholderY + dfaPlaceholderHeight + verticalGap;

    next = new QPushButton("Next", this);
    next->setFont(QFont("Poppins", 10, QFont::Bold));
    next->setStyleSheet("background-color: #16163F; color: white;");
    next->setGeometry(leftX + dfaPlaceholderWidth - navWidth, navY, navWidth, navHeight);

    prev = new QPushButton("Prev", this);
    prev->setFont(QFont("Poppins", 10, QFont::Bold));
    prev->setStyleSheet("background-color: #16163F; color: white;");
    prev->setGeometry(leftX + dfaPlaceholderWidth - 2*navWidth - horizontalGap, navY, navWidth, navHeight);

    // ================= RIGHT SIDE: Tokenization Table =================
    QVBoxLayout* rightLayout = new QVBoxLayout();
    tokenlabel = new QLabel("Token Table", this);
    tokenlabel->setFont(QFont("Poppins", 14, QFont::Bold));
    rightLayout->addWidget(tokenlabel);

    tokenizationtable = new QTableWidget(this);
    tokenizationtable->setColumnCount(4);
    tokenizationtable->setHorizontalHeaderLabels({"Token", "Type", "Line", "Column"});
    tokenizationtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tokenizationtable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tokenizationtable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    rightLayout->addWidget(tokenizationtable);

    // Container widget for the right side
    QWidget* rightContainer = new QWidget(this);
    rightContainer->setLayout(rightLayout);

    // Place it to the right of code editor area
    rightContainer->setGeometry(950, 20, 900, 950);
}

// ================= RUN BUTTON FUNCTION =================
void LexicalAnalysisTab::runLexicalAnalysis()
{
    tokenizationtable->setRowCount(0);
    QString code = userinput->toPlainText();
    QStringList lines = code.split('\n');

    // Regular expressions for Python tokens
    QRegularExpression numberRegex(R"(\b[0-9]+(\.[0-9]+)?\b)");
    QRegularExpression identifierRegex(R"(\b[a-zA-Z_][a-zA-Z0-9_]*\b)");
    QStringList keywords = {"if", "elif", "else", "for", "while", "def", "return"};
    QRegularExpression operatorRegex(R"((\*\*|//|\+=|-=|\*=|/=|==|!=|<=|>=|<|>|\+|-|\*|/|%|=))");
    QRegularExpression delimiterRegex(R"([\{\}\(\)\[\]:\$])");
    QRegularExpression stringRegex(R"((\"[^\\"\n]*\"|'[^'\n]*'))");

    int row = 0;
    for (int lineNum = 0; lineNum < lines.size(); ++lineNum) {
        QString line = lines[lineNum];
        int col = 0;

        while (!line.isEmpty()) {
            line = line.trimmed();
            if (line.isEmpty()) break;

            QRegularExpressionMatch match;

            // Numbers
            match = numberRegex.match(line);
            if (match.hasMatch() && match.capturedStart() == 0) {
                tokenizationtable->insertRow(row);
                tokenizationtable->setItem(row, 0, new QTableWidgetItem(match.captured()));
                tokenizationtable->setItem(row, 1, new QTableWidgetItem("Number"));
                tokenizationtable->setItem(row, 2, new QTableWidgetItem(QString::number(lineNum + 1)));
                tokenizationtable->setItem(row, 3, new QTableWidgetItem(QString::number(col + 1)));
                line = line.mid(match.capturedLength());
                col += match.capturedLength();
                row++;
                continue;
            }

            // String Literals
            match = stringRegex.match(line);
            if (match.hasMatch() && match.capturedStart() == 0) {
                tokenizationtable->insertRow(row);
                tokenizationtable->setItem(row, 0, new QTableWidgetItem(match.captured()));
                tokenizationtable->setItem(row, 1, new QTableWidgetItem("String Literal"));
                tokenizationtable->setItem(row, 2, new QTableWidgetItem(QString::number(lineNum + 1)));
                tokenizationtable->setItem(row, 3, new QTableWidgetItem(QString::number(col + 1)));
                line = line.mid(match.capturedLength());
                col += match.capturedLength();
                row++;
                continue;
            }

            // Delimiters
            match = delimiterRegex.match(line);
            if (match.hasMatch() && match.capturedStart() == 0) {
                tokenizationtable->insertRow(row);
                tokenizationtable->setItem(row, 0, new QTableWidgetItem(match.captured()));
                tokenizationtable->setItem(row, 1, new QTableWidgetItem("Delimiter"));
                tokenizationtable->setItem(row, 2, new QTableWidgetItem(QString::number(lineNum + 1)));
                tokenizationtable->setItem(row, 3, new QTableWidgetItem(QString::number(col + 1)));
                line = line.mid(match.capturedLength());
                col += match.capturedLength();
                row++;
                continue;
            }

            // Operators
            match = operatorRegex.match(line);
            if (match.hasMatch() && match.capturedStart() == 0) {
                tokenizationtable->insertRow(row);
                tokenizationtable->setItem(row, 0, new QTableWidgetItem(match.captured()));
                tokenizationtable->setItem(row, 1, new QTableWidgetItem("Operator"));
                tokenizationtable->setItem(row, 2, new QTableWidgetItem(QString::number(lineNum + 1)));
                tokenizationtable->setItem(row, 3, new QTableWidgetItem(QString::number(col + 1)));
                line = line.mid(match.capturedLength());
                col += match.capturedLength();
                row++;
                continue;
            }

            // Identifiers / Keywords
            match = identifierRegex.match(line);
            if (match.hasMatch() && match.capturedStart() == 0) {
                QString token = match.captured();
                tokenizationtable->insertRow(row);
                tokenizationtable->setItem(row, 0, new QTableWidgetItem(token));
                if (keywords.contains(token))
                    tokenizationtable->setItem(row, 1, new QTableWidgetItem("Keyword"));
                else
                    tokenizationtable->setItem(row, 1, new QTableWidgetItem("Identifier"));
                tokenizationtable->setItem(row, 2, new QTableWidgetItem(QString::number(lineNum + 1)));
                tokenizationtable->setItem(row, 3, new QTableWidgetItem(QString::number(col + 1)));
                line = line.mid(match.capturedLength());
                col += match.capturedLength();
                row++;
                continue;
            }

            // Unknown token
            tokenizationtable->insertRow(row);
            tokenizationtable->setItem(row, 0, new QTableWidgetItem(line.left(1)));
            tokenizationtable->setItem(row, 1, new QTableWidgetItem("Unknown"));
            tokenizationtable->setItem(row, 2, new QTableWidgetItem(QString::number(lineNum + 1)));
            tokenizationtable->setItem(row, 3, new QTableWidgetItem(QString::number(col + 1)));
            line = line.mid(1);
            col += 1;
            row++;
        }
    }

    // ================= EMIT TOKENS =================
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
