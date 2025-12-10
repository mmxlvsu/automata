// OperationParser.cpp
#include "OperationParser.h"
#include <QRegularExpression>
#include <QStringList>

OperationParser::OperationParser(const QString &input)
    : m_input(input)
{
}

void OperationParser::parse()
{
    m_tokens.clear();
    QStringList lines = m_input.split('\n');
    int currentLine = 1;

    QString pattern = R"(\b[a-zA-Z_][a-zA-Z0-9_]*\b)";
    QRegularExpression regex(pattern);

    for (const QString &line : lines) {
        QRegularExpressionMatchIterator it = regex.globalMatch(line);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            ParsedToken token;
            token.lexeme = match.captured();
            token.type = "IDENTIFIER";
            token.line = currentLine;
            m_tokens.append(token);
        }
        ++currentLine;
    }
}

QList<ParsedToken> OperationParser::getParsedTokens() const
{
    return m_tokens;
}