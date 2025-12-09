#ifndef ASSIGNMENTPARSER_H
#define ASSIGNMENTPARSER_H

#include <QString>
#include <QVector>
#include "OperationParser.h"

class AssignmentParser {
public:
    struct ParseResult {
        bool isValid = false;
        QString message;
        QVector<QString> logSteps;
    };

    static ParseResult parse(const QVector<QString>& tokens);
};

#endif // ASSIGNMENTPARSER_H