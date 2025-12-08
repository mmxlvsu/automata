#ifndef ASSIGNMENTPARSER_H
#define ASSIGNMENTPARSER_H

#include <QObject>
#include <QTextEdit>
#include <QList>
#include <QStringList>
#include <stack>

class AssignmentParser : public QObject
{
    Q_OBJECT
public:
    AssignmentParser(QTextEdit* sim, QTextEdit* val, QObject* parent = nullptr);

    void parseTokens(const QList<QList<QString>>& tokens);

private:
    QTextEdit* simulator;
    QTextEdit* validator;
    QList<QList<QString>> tokenList;
    std::stack<QString> pdaStack;
    int idx;

    // CFG parsing functions
    bool parseAssignment();
    bool parseExpression();
    bool parseExprTail();
    bool parseTerm();
    bool parseFactor();

    void appendValidator(const QString& text, bool isError);
    void logPDA(const QString& action);
};

#endif // ASSIGNMENTPARSER_H
