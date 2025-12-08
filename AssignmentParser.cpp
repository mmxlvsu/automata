#include "AssignmentParser.h"

AssignmentParser::AssignmentParser(QTextEdit* sim, QTextEdit* val, QObject* parent)
    : QObject(parent), simulator(sim), validator(val), idx(0)
{
}

void AssignmentParser::logPDA(const QString& action)
{
    QString stackContent;
    std::stack<QString> temp = pdaStack;
    while (!temp.empty()) {
        stackContent = temp.top() + " " + stackContent;
        temp.pop();
    }
    simulator->append("PDA Stack [" + action + "]: " + stackContent);
}

void AssignmentParser::appendValidator(const QString& text, bool isError)
{
    // Use bold Poppins font
    QTextCharFormat format;
    format.setFontFamilies({"Poppins"});
    format.setFontWeight(QFont::Bold);
    format.setForeground(isError ? Qt::red : Qt::darkGreen);

    validator->setCurrentCharFormat(format);
    validator->append(text);
}

void AssignmentParser::parseTokens(const QList<QList<QString>>& tokens)
{
    tokenList = tokens;
    idx = 0;
    validator->clear();
    simulator->clear();

    while (idx < tokenList.size()) {
        // Skip tokens until an identifier (potential assignment)
        while (idx < tokenList.size() && tokenList[idx][1] != "Identifier") idx++;

        if (idx >= tokenList.size()) break;

        pdaStack = std::stack<QString>();
        pdaStack.push("Assignment");
        logPDA("Start");

        int startIdx = idx;
        if (!parseAssignment()) {
            // Only report errors if it looked like an assignment
            appendValidator("Error: Invalid assignment at token " + tokenList[startIdx][0], true);

            // Skip to next identifier to try parsing next statement
            while (idx < tokenList.size() && tokenList[idx][1] != "Identifier") idx++;
        } else {
            appendValidator("Assignment statement parsed successfully.", false);
        }
    }

    simulator->append("Parsing finished.");
}

// ----------------- CFG PARSING -----------------
bool AssignmentParser::parseAssignment()
{
    if (idx >= tokenList.size() || tokenList[idx][1] != "Identifier") return false;
    simulator->append("Found identifier: " + tokenList[idx][0] + " (line " + tokenList[idx][2] + ")");
    idx++;

    if (idx >= tokenList.size() || tokenList[idx][0] != "=") return false;
    simulator->append("Found assignment operator (line " + tokenList[idx][2] + ")");
    idx++;

    if (!parseExpression()) return false;

    return true;
}

bool AssignmentParser::parseExpression()
{
    if (!parseTerm()) return false;
    return parseExprTail();
}

bool AssignmentParser::parseExprTail()
{
    while (idx < tokenList.size() &&
           (tokenList[idx][0] == "+" || tokenList[idx][0] == "-" )) {
        simulator->append("Found operator: " + tokenList[idx][0] + " (line " + tokenList[idx][2] + ")");
        idx++;
        if (!parseTerm()) return false;
    }
    return true;
}

bool AssignmentParser::parseTerm()
{
    if (!parseFactor()) return false;
    while (idx < tokenList.size() &&
           (tokenList[idx][0] == "*" || tokenList[idx][0] == "/" || tokenList[idx][0] == "%")) {
        simulator->append("Found operator: " + tokenList[idx][0] + " (line " + tokenList[idx][2] + ")");
        idx++;
        if (!parseFactor()) return false;
    }
    return true;
}

bool AssignmentParser::parseFactor()
{
    if (idx >= tokenList.size()) return false;

    QString type = tokenList[idx][1];
    QString val = tokenList[idx][0];

    if (type == "Identifier") {
        simulator->append("Found identifier: " + val + " (line " + tokenList[idx][2] + ")");
        idx++;
        return true;
    } else if (type == "Number") {
        simulator->append("Found number: " + val + " (line " + tokenList[idx][2] + ")");
        idx++;
        return true;
    } else if (val == "(") {
        simulator->append("Found delimiter: ( (line " + tokenList[idx][2] + ")");
        pdaStack.push("(");
        logPDA("Push (");
        idx++;
        if (!parseExpression()) return false;

        if (idx >= tokenList.size() || tokenList[idx][0] != ")") return false;
        simulator->append("Found delimiter: ) (line " + tokenList[idx][2] + ")");
        pdaStack.pop();
        logPDA("Pop )");
        idx++;
        return true;
    }

    return false;
}
