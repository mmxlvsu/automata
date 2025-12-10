#ifndef SYNTAXANALYSISTAB_H
#define SYNTAXANALYSISTAB_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QPushButton>

class SyntaxAnalysisTab : public QWidget
{
    Q_OBJECT

public:
    explicit SyntaxAnalysisTab(QWidget *parent = nullptr);
    void updateTokenTable(const QList<QList<QString>>& tokens);

private:
    QLabel* tokenlabel;
    QTableWidget* tokenizationtable;
    QTabWidget* innerTabWidget;

    QWidget *tab1, *tab2, *tab3;
    QTextEdit *tab1Simulator, *tab2Simulator, *tab3Simulator;
    QTextEdit *tab1Validator, *tab2Validator, *tab3Validator;
    QPushButton *tab1Run, *tab2Run, *tab3Run;
};

#endif // SYNTAXANALYSISTAB_H