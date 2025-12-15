// AssignmentParser.h
#ifndef ASSIGNMENTPARSER_H
#define ASSIGNMENTPARSER_H

#include <QString>
#include <QList>
#include "DelimiterParser.h"  // Reuse ParsedToken

class AssignmentParser
{
public:
    AssignmentParser(const QString &input);
    void parse();
    QList<ParsedToken> getParsedTokens() const;

private:
    QString m_input;
    QList<ParsedToken> m_tokens;
};

#endif // ASSIGNMENTPARSER_H