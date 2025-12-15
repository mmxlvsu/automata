// OperationParser.h
#ifndef OPERATIONPARSER_H
#define OPERATIONPARSER_H

#include <QString>
#include <QList>
#include "DelimiterParser.h"  // Reuse ParsedToken

class OperationParser
{
public:
    OperationParser(const QString &input);
    void parse();
    QList<ParsedToken> getParsedTokens() const;

private:
    QString m_input;
    QList<ParsedToken> m_tokens;
};

#endif // OPERATIONPARSER_H