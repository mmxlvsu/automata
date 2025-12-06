#ifndef SYNTAXANALYSISTAB_H
#define SYNTAXANALYSISTAB_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QTabWidget>
#include <QTextEdit>
#include <QPushButton>

class SyntaxAnalysisTab : public QWidget
{
    Q_OBJECT

public:
    explicit SyntaxAnalysisTab(QWidget* parent = nullptr);

public slots:
    void updateTokenTable(const QList<QStringList>& tokens);

private:
    // ================= LEFT SIDE: Token Table =================
    QLabel* tokenlabel;
    QTableWidget* tokenizationtable;

    // ================= RIGHT SIDE: Inner Tabs =================
    QTabWidget* innerTabWidget;
    QWidget* tab1; // Delimiter Parser
    QWidget* tab2; // Assignment Parser
    QWidget* tab3; // Operation Parser

    // ================= SIMULATOR / VALIDATOR / BUTTONS =================
    QTextEdit* tab1Simulator;
    QTextEdit* tab1Validator;
    QPushButton* tab1Run;
    QPushButton* tab1Step;

    QTextEdit* tab2Simulator;
    QTextEdit* tab2Validator;
    QPushButton* tab2Run;
    QPushButton* tab2Step;

    QTextEdit* tab3Simulator;
    QTextEdit* tab3Validator;
    QPushButton* tab3Run;
    QPushButton* tab3Step;
};

#endif // SYNTAXANALYSISTAB_H
