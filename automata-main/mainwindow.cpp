#include "mainwindow.h"
#include "ProjectOverviewTab.h"
#include "LexicalAnalysisTab.h"
#include "SyntaxAnalysisTab.h"
#include "NFASimulatorTab.h" 

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    overviewTab = new ProjectOverviewTab();
    lexicalTab = new LexicalAnalysisTab();
    syntaxTab = new SyntaxAnalysisTab();
    nfaTab = new NFASimulatorTab(); 

    connect(lexicalTab, &LexicalAnalysisTab::tokensReady,
            syntaxTab, &SyntaxAnalysisTab::updateTokenTable);

    tabWidget->addTab(overviewTab, "Project Overview");
    tabWidget->addTab(lexicalTab, "Lexical Analysis");
    tabWidget->addTab(syntaxTab, "Syntax Analysis");
    tabWidget->addTab(nfaTab, "NFA Simulator"); // ✅ Add

    tabWidget->setStyleSheet(R"(
        QTabBar::tab {
            padding: 10px 60px;
            background: rgba(128, 128, 128, 0.3);
            color: rgba(128, 128, 128, 0.6);
            border-top-left-radius: 12px;
            border-top-right-radius: 12px;
            font-family: 'Poppins';
            font-size: 20px;
            font-weight: bold;
            margin-left: 15px;
            margin-top: 15px;
        }
        QTabBar::tab:selected {
            background: #16163F;
            color: white;
        }
    )");

    this->showFullScreen();
}

MainWindow::~MainWindow() {}