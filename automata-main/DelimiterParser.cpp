#include "DelimiterParser.h"

DelimiterParser::ParseResult DelimiterParser::parse(const QVector<QString>& tokens) {
    ParseResult result;
    QStack<QString> stack;
    result.logSteps.append("PDA Simulation Started for Delimiters");

    int pos = 1;
    for (const QString& token : tokens) {
        if (token == "(" || token == "{" || token == "[") {
            stack.push(token);
            result.logSteps.append(QString("Pos %1: Push '%2' onto stack").arg(pos).arg(token));
        }
        else if (token == ")" || token == "}" || token == "]") {
            if (stack.isEmpty()) {
                result.isValid = false;
                result.message = QString("Unexpected closing delimiter '%1' at Pos %2").arg(token).arg(pos);
                result.logSteps.append(result.message);
                return result;
            }

            QString top = stack.pop();
            bool match = (top == "(" && token == ")") ||
                         (top == "{" && token == "}") ||
                         (top == "[" && token == "]");

            if (!match) {
                result.isValid = false;
                result.message = QString("Mismatched delimiters: '%1' vs '%2' at Pos %3")
                                    .arg(top).arg(token).arg(pos);
                result.logSteps.append(result.message);
                return result;
            }
            result.logSteps.append(QString("Pos %1: Matched '%2' and '%3'").arg(pos).arg(top).arg(token));
        }
        else if (token == ";") {
            result.logSteps.append(QString("Pos %1: Found statement terminator ';'").arg(pos));
        }
        ++pos;
    }

    if (!stack.isEmpty()) {
        result.isValid = false;
        result.message = "Unclosed delimiter(s) remaining on stack";
        result.logSteps.append(result.message);
        return result;
    }

    result.isValid = true;
    result.message = "All delimiters are correctly matched.";
    result.logSteps.append("PDA Simulation Completed: Valid Delimiters");
    return result;
}