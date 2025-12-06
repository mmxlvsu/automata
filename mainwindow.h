#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

class LexicalAnalysisTab;
class SyntaxAnalysisTab;
class ProjectOverviewTab;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QTabWidget* tabWidget;
    LexicalAnalysisTab* lexicalTab;
    SyntaxAnalysisTab* syntaxTab;
    ProjectOverviewTab* overviewTab;
};

#endif // MAINWINDOW_H
