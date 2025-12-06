#include "mainwindow.h"
#include "LexicalAnalysisTab.h"
#include "SyntaxAnalysisTab.h"
#include "ProjectOverviewTab.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    // Create tabs
    overviewTab = new ProjectOverviewTab();
    lexicalTab = new LexicalAnalysisTab();
    syntaxTab = new SyntaxAnalysisTab();

    // Connect Lexical tab Run button signal to Syntax tab
    connect(lexicalTab, &LexicalAnalysisTab::tokensReady,
            syntaxTab, &SyntaxAnalysisTab::updateTokenTable);

    // Add tabs
    tabWidget->addTab(overviewTab, "Project Overview");
    tabWidget->addTab(lexicalTab, "Lexical Analysis");
    tabWidget->addTab(syntaxTab, "Syntax Analysis");

    // ================= Tab Styling =================
    tabWidget->setStyleSheet(R"(
        QTabBar::tab {
            padding: 10px 50px;
            background: rgba(128, 128, 128, 0.3); /* grey with 30% opacity */
            color: rgba(128, 128, 128, 0.6);      /* grey with 60% opacity */
            border-top-left-radius: 12px;
            border-top-right-radius: 12px;
            font-family: 'Poppins';
            font-size: 18px;
            font-weight: bold;
        }
        QTabBar::tab:selected {
            background: #16163F;
            color: white;
        }
        QTabBar::tab:!selected {
            background: rgba(128, 128, 128, 0.3);
            color: rgba(128, 128, 128, 0.6);
        }
    )");

    // Fullscreen
    this->showFullScreen();
}

MainWindow::~MainWindow() {}
