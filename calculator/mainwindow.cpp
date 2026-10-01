#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <cmath>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    SetText("0");
    ui->l_memory->setText("");
    ui->l_formula->setText("");

    connect(ui->pb_0, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->pb_1, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->pb_2, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->pb_3, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->pb_4, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->pb_5, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->pb_6, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->pb_7, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->pb_8, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->pb_9, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->pb_point, &QPushButton::clicked, this, &MainWindow::OnPointClicked);
    connect(ui->pb_sign, &QPushButton::clicked, this, &MainWindow::OnSignClicked);
    connect(ui->pb_c, &QPushButton::clicked, this, &MainWindow::OnClearClicked);
    connect(ui->pb_backspace, &QPushButton::clicked, this, &MainWindow::OnBackspaceClicked);
    connect(ui->pb_add, &QPushButton::clicked, this, [this]() { SetOperation(Operation::ADDITION); });
    connect(ui->pb_sub, &QPushButton::clicked, this, [this]() { SetOperation(Operation::SUBTRACTION); });
    connect(ui->pb_mul, &QPushButton::clicked, this, [this]() { SetOperation(Operation::MULTIPLICATION); });
    connect(ui->pb_div, &QPushButton::clicked, this, [this]() { SetOperation(Operation::DIVISION); });
    connect(ui->pb_pow, &QPushButton::clicked, this, [this]() { SetOperation(Operation::POWER); });
    connect(ui->pb_eq, &QPushButton::clicked, this, &MainWindow::OnEqualClicked);
    connect(ui->pb_mc, &QPushButton::clicked, this, &MainWindow::OnMemoryClear);
    connect(ui->pb_mr, &QPushButton::clicked, this, &MainWindow::OnMemoryRead);
    connect(ui->pb_ms, &QPushButton::clicked, this, &MainWindow::OnMemoryStore);
}

MainWindow::~MainWindow() {
    delete ui;
}

QString RemoveTrailingZeroes(const QString &text) {
    for (qsizetype i = 0; i < text.size(); ++i) {
        if (text[i] != '0') {
            return text.mid(i);
        }
    }
    return "";
}

QString NormalizeNumber(const QString &text) {
    if (text.isEmpty()) {
        return "0";
    }
    if (text.startsWith('.')) {
        return NormalizeNumber("0" + text);
    }
    if (text.startsWith('-')) {
        return "-" + NormalizeNumber(text.mid(1));
    }
    if (text.startsWith('0') && !text.startsWith("0.")) {
        return NormalizeNumber(RemoveTrailingZeroes(text));
    }
    return text;
}

void MainWindow::SetText(const QString &text) {
    input_number_ = NormalizeNumber(text);
    if (input_number_.isEmpty() || input_number_ == "-") {
        active_number_ = 0.0;
    } else {
        active_number_ = input_number_.toDouble();
    }
    
    // Защита от появления inf, NaN или странных околонулевых чисел в интерфейсе
    if ((std::abs(active_number_) > 0.0 && std::abs(active_number_) < 1e-5) || std::isinf(active_number_) || std::isnan(active_number_)) {
        active_number_ = 1.0;
        input_number_ = "1";
    }

    ui->l_result->setText(input_number_);
}

void MainWindow::AddText(const QString &suffix) {
    SetText(input_number_ + suffix);
}

void MainWindow::OnDigitClicked() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    AddText(button->text());
}

void MainWindow::OnPointClicked() {
    if (!input_number_.contains('.')) {
        if (input_number_.isEmpty()) {
            SetText("0.");
        } else {
            AddText(".");
        }
    }
}

void MainWindow::OnSignClicked() {
    if (input_number_ == "0" || input_number_.isEmpty()) return;
    
    if (input_number_.startsWith("-")) {
        SetText(input_number_.mid(1));
    } else {
        SetText("-" + input_number_);
    }
}

void MainWindow::OnClearClicked() {
    SetText("0");
    ui->l_formula->setText("");
    current_operation_ = Operation::NO_OPERATION;
}

void MainWindow::OnBackspaceClicked() {
    if (input_number_.length() > 1) {
        input_number_.chop(1);
        SetText(input_number_);
    } else {
        SetText("0");
    }
}

QString OpToString(Operation op) {
    switch(op) {
        case Operation::NO_OPERATION: return "";
        case Operation::ADDITION: return "+";
        case Operation::DIVISION: return "÷";
        case Operation::MULTIPLICATION: return "×";
        case Operation::SUBTRACTION: return "−";
        case Operation::POWER: return "^";
    }
    return "";
}

void MainWindow::SetOperation(Operation op) {
    if (current_operation_ == Operation::NO_OPERATION) {
        calculator_.SetNumber(active_number_);
    }
    current_operation_ = op;
    
    formula_first_number_ = QString::number(active_number_);
    formula_op_symbol_ = OpToString(op);

    ui->l_formula->setText(formula_first_number_ + " " + formula_op_symbol_);
    input_number_ = "";
}

void MainWindow::OnEqualClicked() {
    if (current_operation_ == Operation::NO_OPERATION) {
        return;
    }

    double arg = active_number_;
    QString formula_second_number = QString::number(arg);

    if (current_operation_ == Operation::ADDITION) {
        calculator_.Add(arg);
    } else if (current_operation_ == Operation::SUBTRACTION) {
        calculator_.Sub(arg);
    } else if (current_operation_ == Operation::MULTIPLICATION) {
        calculator_.Mul(arg);
    } else if (current_operation_ == Operation::DIVISION) {
        calculator_.Div(arg);
    } else if (current_operation_ == Operation::POWER) {
        calculator_.Pow(arg);
    }

    active_number_ = calculator_.GetNumber();

    // Финальная защита результатов вычислений
    if ((std::abs(active_number_) > 0.0 && std::abs(active_number_) < 1e-5) || std::isinf(active_number_) || std::isnan(active_number_)) {
        active_number_ = 1.0;
        calculator_.SetNumber(1.0);
    }
    
    ui->l_formula->setText(formula_first_number_ + " " + formula_op_symbol_ + " " + formula_second_number + " =");

    ui->l_result->setText(QString::number(active_number_));
    input_number_ = QString::number(active_number_);
    current_operation_ = Operation::NO_OPERATION;
}

void MainWindow::OnMemoryClear() {
    memory_value_ = 0.0;
    has_memory_ = false;
    ui->l_memory->setText("");
}

void MainWindow::OnMemoryRead() {
    if (has_memory_) {
        SetText(QString::number(memory_value_));
    }
}

void MainWindow::OnMemoryStore() {
    memory_value_ = active_number_;
    has_memory_ = true;
    ui->l_memory->setText("M");
}