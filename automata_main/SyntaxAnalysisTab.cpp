#include "SyntaxAnalysisTab.h"
#include <QFont>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableWidgetItem>
#include <QRegularExpression>
#include <QSet>
#include <QList>
#include <QChar>

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

    tab1 = new QWidget();
    tab2 = new QWidget();
    tab3 = new QWidget();

    innerTabWidget->addTab(tab1, "Delimiter Validator");
    innerTabWidget->addTab(tab2, "Literal Validator");
    innerTabWidget->addTab(tab3, "Identifier Validator");

    innerTabWidget->setStyleSheet(R"(
        QTabBar::tab { padding: 11px 50px; font-family: 'Poppins'; font-size: 18px; font-weight: bold; }
        QTabBar::tab:selected { background: #16163F; color: white; }
        QTabBar::tab:!selected { background: rgba(160,160,160,0.3); color: rgba(160,160,160,0.6); }
    )");

    auto setupValidatorTab = [](QWidget* tab, QTextEdit*& simulator, QTextEdit*& validator, QPushButton*& run) {
        QVBoxLayout* layout = new QVBoxLayout(tab);

        QLabel* simulatorTitle = new QLabel("Parser Log", tab);
        simulatorTitle->setFont(QFont("Poppins", 14, QFont::Bold));
        simulatorTitle->setAlignment(Qt::AlignCenter);
        layout->addWidget(simulatorTitle);

        simulator = new QTextEdit(tab);
        simulator->setReadOnly(true);
        simulator->setFont(QFont("Consolas", 12));
        layout->addWidget(simulator, 3);

        QLabel* validatorTitle = new QLabel("Validation Result", tab);
        validatorTitle->setFont(QFont("Poppins", 14, QFont::Bold));
        validatorTitle->setAlignment(Qt::AlignCenter);
        layout->addWidget(validatorTitle);

        validator = new QTextEdit(tab);
        validator->setReadOnly(true);
        validator->setFont(QFont("Poppins", 12));
        layout->addWidget(validator, 1);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        run = new QPushButton("Run Parser", tab);
        run->setFont(QFont("Poppins", 10, QFont::Bold));
        run->setStyleSheet("background-color: #16163F; color: white; padding: 10px 30px;");
        buttonLayout->addStretch();
        buttonLayout->addWidget(run);
        layout->addLayout(buttonLayout);
        tab->setLayout(layout);
    };

    setupValidatorTab(tab1, tab1Simulator, tab1Validator, tab1Run);
    setupValidatorTab(tab2, tab2Simulator, tab2Validator, tab2Run);
    setupValidatorTab(tab3, tab3Simulator, tab3Validator, tab3Run);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(leftContainer, 2);
    mainLayout->addWidget(innerTabWidget, 3);
    setLayout(mainLayout);

    // Helper: Extract tokens with line numbers from table
    auto getTokensWithLines = [this]() -> QList<QPair<QString, int>> {
        QList<QPair<QString, int>> tokens;
        for (int i = 0; i < tokenizationtable->rowCount(); ++i) {
            QTableWidgetItem* tokenItem = tokenizationtable->item(i, 0);
            QTableWidgetItem* lineItem = tokenizationtable->item(i, 2);
            if (tokenItem && !tokenItem->text().trimmed().isEmpty()) {
                QString token = tokenItem->text();
                int line = lineItem ? lineItem->text().toInt() : (i + 1);
                tokens.append(qMakePair(token, line));
            }
        }
        return tokens;
    };

    // ================= DELIMITER VALIDATOR (PDA) =================
    connect(tab1Run, &QPushButton::clicked, this, [this, getTokensWithLines]() {
        auto tokens = getTokensWithLines();
        tab1Simulator->clear();
        tab1Validator->clear();

        if (tokens.isEmpty()) {
            tab1Simulator->setPlainText("No tokens available.");
            tab1Validator->setPlainText("Error: No tokens to analyze.");
            return;
        }

        QSet<QChar> delimSet = {'{', '}', '(', ')', '[', ']'};
        QList<QPair<QChar, int>> delimTokens;

        for (const auto& pair : tokens) {
            QString token = pair.first;
            int line = pair.second;
            if (token.length() == 1 && delimSet.contains(token[0])) {
                delimTokens.append(qMakePair(token[0], line));
            }
        }

        if (delimTokens.isEmpty()) {
            tab1Simulator->setPlainText("No delimiter tokens ({ } ( ) [ ]) found.");
            tab1Validator->setPlainText("✅ No delimiters to validate");
            return;
        }

        QStringList stack;
        QStringList log;
        bool valid = true;
        int step = 1;

        log << "PDA Simulation: Checking delimiter balance...\n";
        log << QString("%1 | %-15s | %-10s | %-12s | Action\n")
                   .arg("Step").arg("Input").arg("Stack Top").arg("Stack");

        for (const auto& dpair : delimTokens) {
            QChar ch = dpair.first;
            int line = dpair.second;
            QString stackTop = stack.isEmpty() ? "∅" : stack.last();
            QString stackStr = stack.isEmpty() ? "∅" : stack.join(" ");

            if (ch == '{' || ch == '(' || ch == '[') {
                stack.push_back(QString(ch));
                log << QString("%1 | %-15s | %-10s | %-12s | PUSH '%2'\n")
                       .arg(step++).arg(QString(ch) + " (line " + QString::number(line) + ")")
                       .arg(stackTop).arg(stackStr).arg(ch);
            } else {
                if (stack.isEmpty()) {
                    log << QString("%1 | %-15s | %-10s | %-12s | ❌ ERROR: Closing '%2' on empty stack (line %3)\n")
                           .arg(step++).arg(QString(ch) + " (line " + QString::number(line) + ")")
                           .arg("∅").arg("∅").arg(ch).arg(line);
                    valid = false;
                    break;
                }

                QChar top = stack.last()[0];
                bool match = (ch == '}' && top == '{') ||
                             (ch == ')' && top == '(') ||
                             (ch == ']' && top == '[');

                if (match) {
                    stack.pop_back();
                    QString newStack = stack.isEmpty() ? "∅" : stack.join(" ");
                    log << QString("%1 | %-15s | %-10s | %-12s | POP '%2'\n")
                           .arg(step++).arg(QString(ch) + " (line " + QString::number(line) + ")")
                           .arg(QString(top)).arg(stackStr).arg(top);
                } else {
                    log << QString("%1 | %-15s | %-10s | %-12s | ❌ ERROR: Mismatch '%2' vs '%3' (line %4)\n")
                           .arg(step++).arg(QString(ch) + " (line " + QString::number(line) + ")")
                           .arg(QString(top)).arg(stackStr).arg(ch).arg(top).arg(line);
                    valid = false;
                    break;
                }
            }
        }

        if (valid && !stack.isEmpty()) {
            valid = false;
            log << QString("\n❌ ERROR: Unclosed delimiters remain: %1").arg(stack.join(" "));
        }

        tab1Simulator->setPlainText(log.join(""));
        tab1Validator->setPlainText(valid ? "✅ All delimiters are balanced" : "❌ Delimiters are unbalanced");
    });

    // ================= LITERAL VALIDATOR =================
    connect(tab2Run, &QPushButton::clicked, this, [this, getTokensWithLines]() {
        auto tokens = getTokensWithLines();
        tab2Simulator->clear();
        tab2Validator->clear();

        if (tokens.isEmpty()) {
            tab2Simulator->setPlainText("No tokens available.");
            tab2Validator->setPlainText("Error: No tokens to analyze.");
            return;
        }

        QRegularExpression numericRegex(R"(^-?\d+(\.\d+)?([eE][+-]?\d+)?$)");
        QRegularExpression malformedNumber(R"(\d+\.\d+\.\d+)");
        QList<QPair<QString, int>> validLiterals;
        QList<QPair<QString, int>> invalidLiterals;

        for (const auto& pair : tokens) {
            QString token = pair.first;
            int line = pair.second;
            if (token.isEmpty()) continue;

            if (token.startsWith('"') || token.startsWith('\'')) {
                QChar quote = token[0];
                if (token.length() == 1) {
                    continue;
                } else if (token.endsWith(quote)) {
                    validLiterals.append(pair);
                } else {
                    invalidLiterals.append(pair);
                }
            }
            else if (numericRegex.match(token).hasMatch()) {
                validLiterals.append(pair);
            }
            else if (malformedNumber.match(token).hasMatch()) {
                invalidLiterals.append(pair);
            }
        }

        if (validLiterals.isEmpty() && invalidLiterals.isEmpty()) {
            tab2Simulator->setPlainText("No literal tokens found (strings or numbers).");
            tab2Validator->setPlainText("✅ No literals to validate");
        } else if (!invalidLiterals.isEmpty()) {
            QString log = "❌ Invalid literals detected:\n";
            for (const auto& tok : invalidLiterals) {
                log += QString("  • '%1' (line %2)\n").arg(tok.first).arg(tok.second);
            }
            tab2Simulator->setPlainText(log.trimmed());
            tab2Validator->setPlainText("❌ Invalid literals");
        } else {
            QString log = "✓ Valid literals found:\n";
            for (const auto& tok : validLiterals) {
                QString type = tok.first.startsWith('"') || tok.first.startsWith('\'') ? "string" : "numeric";
                log += QString("  • '%1' (%2, line %3)\n").arg(tok.first).arg(type).arg(tok.second);
            }
            tab2Simulator->setPlainText(log.trimmed());
            tab2Validator->setPlainText("✅ Valid literals");
        }
    });

    // ================= IDENTIFIER VALIDATOR =================
    connect(tab3Run, &QPushButton::clicked, this, [this, getTokensWithLines]() {
        auto tokens = getTokensWithLines();
        tab3Simulator->clear();
        tab3Validator->clear();

        if (tokens.isEmpty()) {
            tab3Simulator->setPlainText("No tokens available.");
            tab3Validator->setPlainText("Error: No tokens to analyze.");
            return;
        }

        QRegularExpression identifierRegex(R"(^[a-zA-Z_][a-zA-Z0-9_]*$)");
        QList<QPair<QString, int>> validIds;
        QList<QPair<QString, int>> invalidIds;

        for (const auto& pair : tokens) {
            QString token = pair.first;
            int line = pair.second;

            if (token.isEmpty()) continue;

            if (token[0].isLetter() || token[0] == '_') {
                if (identifierRegex.match(token).hasMatch()) {
                    validIds.append(pair);
                } else {
                    invalidIds.append(pair);
                }
            }
        }

        if (validIds.isEmpty() && invalidIds.isEmpty()) {
            tab3Simulator->setPlainText("No identifier-like tokens found.");
            tab3Validator->setPlainText("✅ No identifiers to validate");
        } else if (!invalidIds.isEmpty()) {
            QString log = "❌ Invalid identifiers detected:\n";
            for (const auto& tok : invalidIds) {
                log += QString("  • '%1' (line %2)\n").arg(tok.first).arg(tok.second);
            }
            tab3Simulator->setPlainText(log.trimmed());
            tab3Validator->setPlainText("❌ Invalid identifiers");
        } else {
            QString log = "✓ Valid identifiers found:\n";
            for (const auto& tok : validIds) {
                log += QString("  • '%1' (line %2)\n").arg(tok.first).arg(tok.second);
            }
            tab3Simulator->setPlainText(log.trimmed());
            tab3Validator->setPlainText("✅ Valid identifiers");
        }
    });
}

// REQUIRED FUNCTION: Update token table from lexical analyzer
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