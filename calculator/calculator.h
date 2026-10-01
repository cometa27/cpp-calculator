#pragma once
#include <cmath>

class Calculator {
public:
    Calculator();
    
    void SetNumber(double number);
    double GetNumber() const;
    
    void Add(double arg);
    void Sub(double arg);
    void Mul(double arg);
    void Div(double arg);
    void Pow(double arg);
    
    void Clear();

private:
    double current_number_;
};
