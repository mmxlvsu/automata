#include "AssignmentParser.h"

AssignmentParser::ParseResult AssignmentParser::parse(const QVector<QString>& tokens) {
    ParseResult result;
    result.logSteps.append("PDA Simulation Started for Assignment Statement");

    if (tokens.isEmpty()) {
        result.message = "Input is empty";
        result.logSteps.append(result.message);
        return result;
    }

    int pos = 1;

    // Step 1: Check identifier (must start with letter or _)
    QString first = tokens[0];
    if (first.isEmpty() || (!first.at(0).isLetter() && first.at(0) != '_')) {
        result.message = "Expected identifier at start of assignment";
        result.logSteps.append(QString("Pos %1: Invalid start token '%2'").arg(pos).arg(first));
        return result;
    }
    result.logSteps.append(QString("Pos %1: Accepted identifier '%2'").arg(pos).arg(first));
    pos++;

    // Step 2: Check '='
    if (pos > tokens.size() || tokens[pos - 1] != "=") {
        result.message = "Expected '=' after identifier";
        result.logSteps.append(QString("Pos %1: Expected '=', got '%2'")
                               .arg(pos).arg(pos <= tokens.size() ? tokens[pos - 1] : "EOF"));
        return result;
    }
    result.logSteps.append(QString("Pos %1: Accepted '='").arg(pos));
    pos++;

    // Step 3: Extract expression (everything after = until end)
    QVector<QString> exprTokens;
    while (pos <= tokens.size()) {
        exprTokens.append(tokens[pos - 1]);
        pos++;
    }

    if (exprTokens.isEmpty()) {
        result.message = "Expected expression after '='";
        result.logSteps.append("No expression found after '='");
        return result;
    }

    result.logSteps.append(QString("Extracted expression tokens: %1").arg(exprTokens.join(" ")));

    // Step 4: Parse expression
    auto opResult = OperationParser::parse(exprTokens);
    for (const QString& step : opResult.logSteps) {
        result.logSteps.append("  → " + step);
    }

    if (!opResult.isValid) {
        result.message = "Invalid expression in assignment: " + opResult.message;
        return result;
    }

    // Step 5: No semicolon required (Python-style)
    result.isValid = true;
    result.message = "Valid assignment statement";
    result.logSteps.append("PDA Simulation Completed: Valid Assignment");
    return result;
}