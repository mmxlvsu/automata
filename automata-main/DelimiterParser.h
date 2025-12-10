// DelimiterParser.h
#ifndef DELIMITERPARSER_H
#define DELIMITERPARSER_H

#include <QString>
#include <QList>

struct ParsedToken {
    QString lexeme;
    QString type;
    int line;
};

class DelimiterParser
{
public:
    DelimiterParser(const QString &input);
    void parse();
    QList<ParsedToken> getParsedTokens() const;

private:
    QString m_input;
    QList<ParsedToken> m_tokens;
};

#endif // DELIMITERPARSER_H