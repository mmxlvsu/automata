#include "ProjectOverviewTab.h"
#include <QVBoxLayout>
#include <QFont>
#include <QTextCursor>
#include <QTextCharFormat>

ProjectOverviewTab::ProjectOverviewTab(QWidget* parent)
    : QWidget(parent)
{
    content = new QTextEdit(this);
    content->setReadOnly(true);
    content->setFont(QFont("Poppins", 12));
    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QTextCursor cursor(content->document());
    QTextCharFormat titleFormat;
    titleFormat.setFont(QFont("Poppins", 20, QFont::Bold));

    QTextCharFormat subtitleFormat;
    subtitleFormat.setFont(QFont("Poppins", 18, QFont::Bold));

    QTextCharFormat normalFormat;
    normalFormat.setFont(QFont("Poppins", 15));

    // Insert main title
    cursor.insertText("\n\nCS311: Automata Theory and Formal Languages\n\n", titleFormat);

    // Project Title
    cursor.insertText("\nProject Title:\n", subtitleFormat);
    cursor.insertText("Python-Based Compiler Front-End Simulator and Syntax/Lexical Validator\n\n\n\n", normalFormat);

    cursor.insertText("Section:\n", subtitleFormat);
    cursor.insertText("CS3A\n\n", normalFormat);

        cursor.insertText("Team Members:\n", subtitleFormat);
    cursor.insertText("Adanza, Aaron\nGultiano, Kathleen Grace\nJison, Remar\nLaplap, Mariel\n\n\n\n", normalFormat);

    // Description
    cursor.insertText("Description:\n", subtitleFormat);
    cursor.insertText(
        "This project implements a Python-like compiler front-end simulator with lexical and syntax validation. "
        "It demonstrates key concepts from Automata Theory by simulating the behavior of finite automata (FA) for lexical analysis "
        "and pushdown automata (PDA) for syntax validation.\n\n", normalFormat);

    // Components
    cursor.insertText("Components:\n", subtitleFormat);
    cursor.insertText(
        "1. Lexical Analysis\n"
        "   - Scans multi-line input code to identify tokens: Numbers, Identifiers, Keywords, Operators, Delimiters, and String Literals.\n"
        "   - Tokens are recognized using regular expressions and modeled via a Deterministic Finite Automaton (DFA).\n"
        "   - The GUI displays the token table and highlights DFA states step-by-step during scanning.\n\n", normalFormat);

    cursor.insertText(
        "2. Syntax Analysis\n"
        "   - Validates structural correctness of the code, such as balanced parentheses, brackets, braces, and simple statements.\n"
        "   - Implemented parsers include:\n"
        "       - Delimiter Parser – checks for nested {}, (), []\n"
        "       - Assignment Parser – validates assignment statements and basic expressions\n"
        "       - Operation/Expression Parser – partially implemented for arithmetic expressions\n"
        "   - Uses a Pushdown Automaton (PDA) to simulate parsing and display the stack operations in real-time.\n\n", normalFormat);

    // GUI Features
    cursor.insertText("GUI Features:\n", subtitleFormat);
    cursor.insertText(
        "- Two main sections: Token Table on the left, Parser Tabs on the right.\n"
        "- Each parser has its own simulator log and validation output.\n"
        "- Step-by-step visualization helps users trace how tokens are recognized and how parsing occurs.\n\n", normalFormat);

    // Scope & Purpose
    cursor.insertText("Scope & Purpose:\n", subtitleFormat);
    cursor.insertText(
        "- Provides an interactive educational tool for understanding compiler front-end design.\n"
        "- Focuses on structural validation rather than full execution or semantic analysis.\n"
        "- Demonstrates how lexical and syntax analysis work together in a real compiler.\n\n", normalFormat);

    // Significance
    cursor.insertText("Significance:\n", subtitleFormat);
    cursor.insertText(
        "- Bridges theoretical automata concepts with practical compiler design.\n"
        "- Helps users understand tokenization, DFA simulation, PDA parsing, and basic Python-like syntax checking.\n\n", normalFormat);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(content);
}
