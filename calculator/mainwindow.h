#pragma once

#include "calculator.h"

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum class Operation {
    NO_OPERATION,
    MULTIPLICATION,
    DIVISION,
    SUBTRACTION,
    ADDITION,
    POWER,
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void OnDigitClicked();
    void OnEqualClicked();
    void OnClearClicked();
    void OnPointClicked();
    void OnSignClicked();
    void OnBackspaceClicked();

    void OnMemoryClear();
    void OnMemoryRead();
    void OnMemoryStore();

private:
    void SetText(const QString &text);
    void AddText(const QString &suffix);
    void SetOperation(Operation op);

    Ui::MainWindow* ui;
    Calculator calculator_;
    QString input_number_ = "0";
    double active_number_ = 0.0;
    Operation current_operation_ = Operation::NO_OPERATION;

    double memory_value_ = 0.0;
    bool has_memory_ = false;

    QString formula_first_number_;
    QString formula_op_symbol_;
};