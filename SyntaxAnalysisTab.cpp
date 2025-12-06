#include "SyntaxAnalysisTab.h"
#include <QFont>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>

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

    // ================= FUNCTION TO CREATE TAB LAYOUT =================
    auto setupParserTab = [](QWidget* tab, QTextEdit*& simulator, QTextEdit*& validator,
                             QPushButton*& run, QPushButton*& step) {
        QVBoxLayout* layout = new QVBoxLayout(tab);

        simulator = new QTextEdit(tab);
        simulator->setReadOnly(true);
        simulator->setFont(QFont("Consolas", 12));
        simulator->setPlaceholderText("PDA Simulator log...");
        layout->addWidget(simulator, 3);

        validator = new QTextEdit(tab);
        validator->setReadOnly(true);
        validator->setFont(QFont("Poppins", 12));
        validator->setPlaceholderText("Validation results...");
        layout->addWidget(validator, 1);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        run = new QPushButton("Run", tab);
        run->setFont(QFont("Poppins", 10, QFont::Bold));
        run->setStyleSheet("background-color: #16163F; color: white; padding: 10px 30px;");

        step = new QPushButton("Step", tab);
        step->setFont(QFont("Poppins", 10, QFont::Bold));
        step->setStyleSheet("background-color: #16163F; color: white; padding: 10px 30px;");

        buttonLayout->addStretch();
        buttonLayout->addWidget(run);
        buttonLayout->addWidget(step);
        layout->addLayout(buttonLayout);
    };

    // ================= SETUP EACH TAB =================
    setupParserTab(tab1, tab1Simulator, tab1Validator, tab1Run, tab1Step);
    setupParserTab(tab2, tab2Simulator, tab2Validator, tab2Run, tab2Step);
    setupParserTab(tab3, tab3Simulator, tab3Validator, tab3Run, tab3Step);

    // ================= MAIN LAYOUT =================
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(leftContainer, 2);
    mainLayout->addWidget(innerTabWidget, 3);
    setLayout(mainLayout);
}

// ================= UPDATE TOKEN TABLE =================
void SyntaxAnalysisTab::updateTokenTable(const QList<QList<QString>>& tokens)
{
    tokenizationtable->setRowCount(0);

    for (int i = 0; i < tokens.size(); ++i) {
        tokenizationtable->insertRow(i);
        tokenizationtable->setItem(i, 0, new QTableWidgetItem(tokens[i][0]));
        tokenizationtable->setItem(i, 1, new QTableWidgetItem(tokens[i][1]));
        tokenizationtable->setItem(i, 2, new QTableWidgetItem(tokens[i][2]));
        tokenizationtable->setItem(i, 3, new QTableWidgetItem(tokens[i][3]));
    }

    tab1Simulator->clear();
    tab1Validator->clear();
    tab2Simulator->clear();
    tab2Validator->clear();
    tab3Simulator->clear();
    tab3Validator->clear();
}
