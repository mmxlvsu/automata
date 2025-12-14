#ifndef NFASIMULATORTAB_H
#define NFASIMULATORTAB_H

#include <QWidget>
#include <QPushButton>
#include <QGraphicsView>
#include <QComboBox>

class NFASimulatorTab : public QWidget
{
    Q_OBJECT

public:
    explicit NFASimulatorTab(QWidget *parent = nullptr);

private slots:
    void onPatternChanged(int index);
    void drawNFA();

private:
    enum PatternType {
        Identifier,
        Integer,
        Float,
        StringLiteral  // 🔹 NEW
    };

    QComboBox* patternSelector;
    QGraphicsView* graphicsView;
    QPushButton* drawButton;
    PatternType currentPattern = Identifier;
};

#endif // NFASIMULATORTAB_H