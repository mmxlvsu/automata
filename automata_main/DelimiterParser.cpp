// DelimiterParser.cpp
#include "DelimiterParser.h"
#include <QRegularExpression>
#include <QStringList>

DelimiterParser::DelimiterParser(const QString &input)
    : m_input(input)
{
}

void DelimiterParser::parse()
{
    m_tokens.clear();
    QStringList lines = m_input.split('\n');
    int currentLine = 1;

    QString delimiterPattern = R"([;\{\}\(\)\[\],.:])";
    QRegularExpression regex(delimiterPattern);

    for (const QString &line : lines) {
        QRegularExpressionMatchIterator it = regex.globalMatch(line);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            ParsedToken token;
            token.lexeme = match.captured();
            token.type = "DELIMITER";
            token.line = currentLine;
            m_tokens.append(token);
        }
        ++currentLine;
    }
}

QList<ParsedToken> DelimiterParser::getParsedTokens() const
{
    return m_tokens;
}