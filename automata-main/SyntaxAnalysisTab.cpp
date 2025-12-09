#include "SyntaxAnalysisTab.h"
#include "AssignmentParser.h"
#include "DelimiterParser.h"
#include "OperationParser.h"
#include <QFont>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableWidgetItem>
#include <QList>

SyntaxAnalysisTab::SyntaxAnalysisTab(QWidget* parent)
    : QWidget(parent)
{
    // ================= LEFT SIDE: Token Table =================
    tokenlabel = new QLabel("Token Table", this);
    tokenlabel->setFont(QFont("Poppins", 14, QFont::Bold));

    tokenizationtable = new QTableWidget(this);
    tokenizationtable->setColumnCount(4);
    tokenizationtable->setHorizontalHeaderLabels({"Token", "Type", "Line", "Column"});
    tokenizationtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tokenizationtable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tokenizationtable->verticalHeader()->setVisible(false);

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(tokenlabel);
    leftLayout->addWidget(tokenizationtable);

    QWidget* leftContainer = new QWidget(this);
    leftContainer->setLayout(leftLayout);

    // ================= RIGHT SIDE: Inner Tabs =================
    innerTabWidget = new QTabWidget(this);

    tab1 = new QWidget(); // Delimiter Parser
    tab2 = new QWidget(); // Assignment Parser
    tab3 = new QWidget(); // Operation Parser

    innerTabWidget->addTab(tab1, "Delimiter Parser");
    innerTabWidget->addTab(tab2, "Assignment Parser");
    innerTabWidget->addTab(tab3, "Operation Parser");

    innerTabWidget->setStyleSheet(R"(
        QTabBar::tab { padding: 10px 50px; font-family: 'Poppins'; font-size: 18px; font-weight: bold; }
        QTabBar::tab:selected { background: #16163F; color: white; }
        QTabBar::tab:!selected { background: rgba(160,160,160,0.3); color: rgba(160,160,160,0.6); }
    )");

    auto setupParserTab = [](QWidget* tab, QTextEdit*& simulator, QTextEdit*& validator, QPushButton*& run) {
        QVBoxLayout* layout = new QVBoxLayout(tab);

        QLabel* simulatorTitle = new QLabel("PDA Simulator Log:", tab);
        simulatorTitle->setFont(QFont("Poppins", 14, QFont::Bold));
        simulatorTitle->setAlignment(Qt::AlignCenter);
        layout->addWidget(simulatorTitle);

        simulator = new QTextEdit(tab);
        simulator->setReadOnly(true);
        simulator->setFont(QFont("Consolas", 12));
        layout->addWidget(simulator, 3);

        QLabel* validatorTitle = new QLabel("Validation Results", tab);
        validatorTitle->setFont(QFont("Poppins", 14, QFont::Bold));
        validatorTitle->setAlignment(Qt::AlignCenter);
        layout->addWidget(validatorTitle);

        validator = new QTextEdit(tab);
        validator->setReadOnly(true);
        validator->setFont(QFont("Poppins", 12));
        layout->addWidget(validator, 1);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        run = new QPushButton("Run", tab);
        run->setFont(QFont("Poppins", 10, QFont::Bold));
        run->setStyleSheet("background-color: #16163F; color: white; padding: 10px 30px;");
        buttonLayout->addStretch();
        buttonLayout->addWidget(run);
        layout->addLayout(buttonLayout);
    };

    setupParserTab(tab1, tab1Simulator, tab1Validator, tab1Run);
    setupParserTab(tab2, tab2Simulator, tab2Validator, tab2Run);
    setupParserTab(tab3, tab3Simulator, tab3Validator, tab3Run);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(leftContainer, 2);
    mainLayout->addWidget(innerTabWidget, 3);
    setLayout(mainLayout);

    // ================= CONNECT RUN BUTTONS =================

    // DELIMITER PARSER
    connect(tab1Run, &QPushButton::clicked, this, [this]() {
        QVector<QString> tokens;
        for (int i = 0; i < tokenizationtable->rowCount(); ++i) {
            QTableWidgetItem* item = tokenizationtable->item(i, 0);
            if (item) tokens.append(item->text());
        }

        tab1Simulator->clear();
        tab1Validator->clear();

        if (tokens.isEmpty()) {
            tab1Validator->setPlainText("Error: No tokens to parse.");
            return;
        }

        auto result = DelimiterParser::parse(tokens);
        for (const QString& step : result.logSteps) {
            tab1Simulator->append(step);
        }
        tab1Validator->setPlainText(result.isValid ? "✅ Valid delimiters" : "❌ Invalid: " + result.message);
    });

    // ASSIGNMENT PARSER - Fixed logic
    connect(tab2Run, &QPushButton::clicked, this, [this]() {
        QVector<QString> allTokens;
        for (int i = 0; i < tokenizationtable->rowCount(); ++i) {
            QTableWidgetItem* item = tokenizationtable->item(i, 0);
            if (item) allTokens.append(item->text());
        }

        tab2Simulator->clear();
        tab2Validator->clear();

        if (allTokens.isEmpty()) {
            tab2Validator->setPlainText("Error: No tokens to parse.");
            return;
        }

        int statementCount = 0;
        int pos = 0;

        while (pos < allTokens.size()) {
            // Check for valid identifier (must start with letter or underscore)
            if (!allTokens[pos].isEmpty() && 
                (allTokens[pos].at(0).isLetter() || allTokens[pos].at(0) == '_')) {
                
                QString identifier = allTokens[pos];
                pos++; // Move past identifier

                // Check if next token is '='
                if (pos < allTokens.size() && allTokens[pos] == "=") {
                    pos++; // Move past '='
                    statementCount++;

                    // Extract full expression: take all tokens until next identifier or end
                    QVector<QString> exprTokens;
                    int exprStart = pos;
                    while (pos < allTokens.size()) {
                        // Stop if we find a new potential assignment (new identifier)
                        if (!allTokens[pos].isEmpty() && 
                            (allTokens[pos].at(0).isLetter() || allTokens[pos].at(0) == '_')) {
                            break;
                        }
                        exprTokens.append(allTokens[pos]);
                        pos++;
                    }

                    // Create minimal assignment: [id, '='] for structure validation
                    QVector<QString> assignmentTokens;
                    assignmentTokens << identifier << "=";

                    auto result = AssignmentParser::parse(assignmentTokens);

                    // Log assignment parsing
                    tab2Simulator->append(QString("\n=== Statement %1 ===").arg(statementCount));
                    for (const QString& step : result.logSteps) {
                        tab2Simulator->append(step);
                    }

                    // Now validate the expression part
                    if (!exprTokens.isEmpty()) {
                        tab2Simulator->append("→ Expression tokens: " + exprTokens.join(" "));
                        auto opResult = OperationParser::parse(exprTokens);
                        for (const QString& step : opResult.logSteps) {
                            tab2Simulator->append("  → " + step);
                        }
                        if (!opResult.isValid) {
                            result.isValid = false;
                            result.message = "Expression invalid: " + opResult.message;
                        }
                    } else {
                        tab2Simulator->append("→ Warning: Empty expression");
                    }

                    QString status = result.isValid ? "✅ Valid" : "❌ Invalid: " + result.message;
                    tab2Validator->append(status);
                    continue;
                }
            }
            pos++; // Skip non-assignment tokens
        }

        if (statementCount == 0) {
            tab2Validator->setPlainText("No assignment statements detected.");
        }
    });

    // OPERATION PARSER - Fixed logic
    connect(tab3Run, &QPushButton::clicked, this, [this]() {
        QVector<QString> allTokens;
        for (int i = 0; i < tokenizationtable->rowCount(); ++i) {
            QTableWidgetItem* item = tokenizationtable->item(i, 0);
            if (item) allTokens.append(item->text());
        }

        tab3Simulator->clear();
        tab3Validator->clear();

        if (allTokens.isEmpty()) {
            tab3Validator->setPlainText("Error: No tokens to parse.");
            return;
        }

        // Find first assignment and extract its expression
        QVector<QString> exprTokens;
        bool found = false;

        for (int i = 0; i < allTokens.size() - 1; ++i) {
            if ((allTokens[i].at(0).isLetter() || allTokens[i].at(0) == '_') && 
                allTokens[i + 1] == "=") {
                // Found assignment, extract expression after '='
                for (int j = i + 2; j < allTokens.size(); ++j) {
                    if ((allTokens[j].at(0).isLetter() || allTokens[j].at(0) == '_')) {
                        break; // Stop at next assignment
                    }
                    exprTokens.append(allTokens[j]);
                }
                found = true;
                break;
            }
        }

        if (!found) {
            // Fallback: parse entire input as expression
            auto result = OperationParser::parse(allTokens);
            for (const QString& step : result.logSteps) {
                tab3Simulator->append(step);
            }
            tab3Validator->setPlainText(result.isValid ? "✅ Valid expression" : "❌ Invalid: " + result.message);
            return;
        }

        if (exprTokens.isEmpty()) {
            tab3Validator->setPlainText("No expression found after '='.");
            return;
        }

        auto result = OperationParser::parse(exprTokens);
        for (const QString& step : result.logSteps) {
            tab3Simulator->append(step);
        }
        tab3Validator->setPlainText(result.isValid ? "✅ Valid expression" : "❌ Invalid: " + result.message);
    });
}

// Required function for lexical analysis integration
void SyntaxAnalysisTab::updateTokenTable(const QList<QList<QString>>& tokens)
{
    tokenizationtable->setRowCount(tokens.size());
    for (int i = 0; i < tokens.size(); ++i) {
        const auto& row = tokens[i];
        for (int j = 0; j < qMin(row.size(), 4); ++j) {
            tokenizationtable->setItem(i, j, new QTableWidgetItem(row[j]));
        }
    }
}