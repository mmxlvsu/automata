#include "ProjectOverviewTab.h"
#include <QVBoxLayout>
#include <QFont>

ProjectOverviewTab::ProjectOverviewTab(QWidget* parent)
    : QWidget(parent)
{
    content = new QTextEdit(this);
    content->setReadOnly(true);
    content->setText(
        "=== Project Overview ===\n\n"
        "This project is a front-end compiler development.\n"
        "It contains three main tabs: Project Overview, Lexical Analysis, and Syntax Analysis."
    );
    content->setFont(QFont("Arial Rounded MT Bold", 12));
    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(content);
}
