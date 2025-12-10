// AssignmentParser.cpp
#include "AssignmentParser.h"
#include <QRegularExpression>
#include <QStringList>

AssignmentParser::AssignmentParser(const QString &input)
    : m_input(input)
{
}

void AssignmentParser::parse()
{
    m_tokens.clear();
    QStringList lines = m_input.split('\n');
    int currentLine = 1;

    QString pattern = R"("([^"\\]|\\.)*"|[']([^'\\]|\\.)*[']|-?\b\d+(\.\d+)?([eE][+-]?\d+)?\b)";
    QRegularExpression regex(pattern);
    regex.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);

    for (const QString &line : lines) {
        QRegularExpressionMatchIterator it = regex.globalMatch(line);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            QString lexeme = match.captured();

            ParsedToken token;
            token.lexeme = lexeme;
            token.line = currentLine;
            token.type = (lexeme.startsWith('"') || lexeme.startsWith('\''))
                         ? "STRING_LITERAL" : "NUMERIC_LITERAL";
            m_tokens.append(token);
        }
        ++currentLine;
    }
}

QList<ParsedToken> AssignmentParser::getParsedTokens() const
{
    return m_tokens;
}