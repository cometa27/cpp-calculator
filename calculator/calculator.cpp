#include "calculator.h"
#include <cmath>

Calculator::Calculator() : current_number_(0.0) {}

void Calculator::SetNumber(double number) {
    current_number_ = number;
}

double Calculator::GetNumber() const {
    return current_number_;
}

void Calculator::Add(double arg) {
    current_number_ += arg;
}

void Calculator::Sub(double arg) {
    current_number_ -= arg;
}

void Calculator::Mul(double arg) {
    current_number_ *= arg;
}

void Calculator::Div(double arg) {
    if (arg == 0.0 || std::abs(arg) < 1e-5 || std::isinf(arg) || std::isnan(arg)) {
        current_number_ = 1.0; 
    } else {
        current_number_ /= arg;
    }
}

void Calculator::Pow(double arg) {
    current_number_ = std::pow(current_number_, arg);
}

void Calculator::Clear() {
    current_number_ = 0.0;
}
