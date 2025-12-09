#ifndef OPERATIONPARSER_H
#define OPERATIONPARSER_H

#include <QString>
#include <QVector>

class OperationParser {
public:
    struct ParseResult {
        bool isValid = false;
        QString message;
        QVector<QString> logSteps;
    };

    static ParseResult parse(const QVector<QString>& tokens);
};

#endif // OPERATIONPARSER_H