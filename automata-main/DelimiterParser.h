#ifndef DELIMITERPARSER_H
#define DELIMITERPARSER_H

#include <QString>
#include <QVector>
#include <QStack>

class DelimiterParser {
public:
    struct ParseResult {
        bool isValid = false;
        QString message;
        QVector<QString> logSteps;
    };

    static ParseResult parse(const QVector<QString>& tokens);
};

#endif // DELIMITERPARSER_H