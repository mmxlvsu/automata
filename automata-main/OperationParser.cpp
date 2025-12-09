#include "OperationParser.h"
#include <QStack>

OperationParser::ParseResult OperationParser::parse(const QVector<QString>& tokens) {
    ParseResult result;
    result.logSteps.append("PDA Simulation Started for Expression");

    if (tokens.isEmpty()) {
        result.message = "Empty expression";
        result.logSteps.append(result.message);
        return result;
    }

    QStack<QString> valueStack;
    QStack<QString> opStack;
    int pos = 1;

    auto precedence = [](const QString& op) -> int {
        if (op == "+" || op == "-") return 1;
        if (op == "*" || op == "/") return 2;
        return 0;
    };

    for (const QString& token : tokens) {
        result.logSteps.append(QString("Pos %1: Processing token '%2'").arg(pos).arg(token));

        if (token == "(") {
            opStack.push(token);
            result.logSteps.append("  → Push '(' to operator stack");
        }
        else if (token == ")") {
            while (!opStack.isEmpty() && opStack.top() != "(") {
                if (valueStack.size() < 2) {
                    result.message = "Insufficient operands for operator";
                    result.logSteps.append("  → Error: Not enough operands to apply operator");
                    return result;
                }
                valueStack.pop(); valueStack.pop();
                valueStack.push("expr");
                QString op = opStack.pop();
                result.logSteps.append(QString("  → Applied operator '%1'").arg(op));
            }
            if (opStack.isEmpty()) {
                result.message = "Unmatched closing parenthesis ')'";
                result.logSteps.append("  → Error: No matching '(' for ')'");
                return result;
            }
            opStack.pop();
            result.logSteps.append("  → Matched ')' with '('");
        }
        else if (token == "+" || token == "-" || token == "*" || token == "/") {
            while (!opStack.isEmpty() && opStack.top() != "(" &&
                   precedence(opStack.top()) >= precedence(token)) {
                if (valueStack.size() < 2) {
                    result.message = "Malformed expression near operator";
                    result.logSteps.append("  → Error: Invalid operator context");
                    return result;
                }
                valueStack.pop(); valueStack.pop();
                valueStack.push("expr");
                QString op = opStack.pop();
                result.logSteps.append(QString("  → Applied higher-precedence operator '%1'").arg(op));
            }
            opStack.push(token);
            result.logSteps.append(QString("  → Push operator '%1' to stack").arg(token));
        }
        else {
            // Check for string literals
            bool isValidOperand = false;
            if (token.startsWith("\"") && token.endsWith("\"")) {
                isValidOperand = true;
                result.logSteps.append(QString("  → Recognized string literal '%1'").arg(token));
            }
            else if (!token.isEmpty()) {
                QChar c = token.at(0);
                if (c.isLetter() || c == '_' || c.isDigit()) {
                    isValidOperand = true;
                }
            }

            if (!isValidOperand) {
                result.message = QString("Invalid token '%1' — not allowed in expression").arg(token);
                result.logSteps.append(QString("  → Error: '%1' is not a valid operand").arg(token));
                return result;
            }

            valueStack.push(token);
            result.logSteps.append(QString("  → Push operand '%1' to value stack").arg(token));
        }
        pos++;
    }

    // Final reduction
    while (!opStack.isEmpty()) {
        if (opStack.top() == "(") {
            result.message = "Unmatched opening parenthesis '('";
            result.logSteps.append("  → Error: '(' was never closed");
            return result;
        }
        if (valueStack.size() < 2) {
            result.message = "Incomplete expression — missing operand";
            result.logSteps.append("  → Error: Not enough operands for final operator");
            return result;
        }
        valueStack.pop(); valueStack.pop();
        valueStack.push("expr");
        QString op = opStack.pop();
        result.logSteps.append(QString("  → Applied final operator '%1'").arg(op));
    }

    if (valueStack.size() != 1) {
        result.message = "Expression did not reduce to a single value";
        result.logSteps.append("  → Error: Final stack has more than one value");
        return result;
    }

    result.isValid = true;
    result.message = "Valid arithmetic expression";
    result.logSteps.append("PDA Simulation Completed: Valid Expression");
    return result;
}