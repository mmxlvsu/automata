#ifndef OPERATIONPARSER_H
#define OPERATIONPARSER_H

#include <QTextEdit>
#include <QList>
#include <QStringList>

class OperationParser
{
public:
    OperationParser(QTextEdit* simulator, QTextEdit* validator)
        : simulator(simulator), validator(validator) {}

    void parseTokens(const QList<QStringList>& tokens) {
        simulator->clear();
        validator->clear();
        simulator->append("Operation Parser not implemented yet.");
    }

private:
    QTextEdit* simulator;
    QTextEdit* validator;
};

#endif // OPERATIONPARSER_H
