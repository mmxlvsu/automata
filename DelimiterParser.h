#ifndef DELIMITERPARSER_H
#define DELIMITERPARSER_H

#include <QTextEdit>
#include <QList>
#include <QStringList>

class DelimiterParser
{
public:
    DelimiterParser(QTextEdit* simulator, QTextEdit* validator)
        : simulator(simulator), validator(validator) {}

    void parseTokens(const QList<QStringList>& tokens) {
        simulator->clear();
        validator->clear();
        simulator->append("Delimiter Parser not implemented yet.");
    }

private:
    QTextEdit* simulator;
    QTextEdit* validator;
};

#endif // DELIMITERPARSER_H
