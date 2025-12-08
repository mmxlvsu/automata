#ifndef LEXICALANALYSISTAB_H
#define LEXICALANALYSISTAB_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

class LexicalAnalysisTab : public QWidget
{
    Q_OBJECT

public:
    LexicalAnalysisTab(QWidget* parent = nullptr);
    QTableWidget* getTokenizationTable() const { return tokenizationtable; }

signals:
    void tokensReady(const QList<QStringList>& tokens);

private slots:
    void runLexicalAnalysis();

private:
    QLabel* userlabel;
    QTextEdit* userinput;
    QPushButton* run;

    QLabel* dfa;
    QGraphicsScene* dfaScene;
    QGraphicsView* dfaView;

    QLabel* tokenlabel;
    QTableWidget* tokenizationtable;
};

#endif // LEXICALANALYSISTAB_H
