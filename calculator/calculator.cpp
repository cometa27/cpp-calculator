#include "calculator.h"
#include <cmath>
#include <string>

bool ReadNumber(Number& result){
    if(std::cin >> result){
        return true;
    }
    std::cerr << "Error: Numeric operand expected\n";
    return false;
}

bool RunCalculatorCycle(){
    Number currentNumber = 0; 
    Number memory = 0;
    bool hasmemory = false;
    std::string command;

    if (!ReadNumber(currentNumber)){
        return false;
    }
    while (std::cin >> command){
        
        std::string operations[5] = {"+", "-", "*", "/", "**"};
        bool isBinary = false;
        
        for (int i = 0; i < 5; i++) {
            if (command == operations[i]) {
                isBinary = true;
                break;
            }
        }

        if (isBinary){
            Number arg;
            if (!ReadNumber(arg)){
                return false;
            }
            if (command == "+"){
                currentNumber += arg;
            } else if (command == "-"){
                currentNumber -= arg;
            } else if (command == "*"){
                currentNumber *= arg;
            } else if (command == "/"){
                currentNumber /= arg;
            } else if (command == "**"){
                currentNumber = std::pow(currentNumber, arg);
            }
        } else if (command == ":"){
            Number arg;
            if (!ReadNumber(arg)){
                return false;
            }
            currentNumber = arg;

        } else if (command == "c"){
            currentNumber = 0;

        } else if (command == "="){
            std::cout << currentNumber << "\n";

        } else if (command == "s"){
            memory = currentNumber;
            hasmemory = true;
                
        } else if (command == "l"){
            if (!hasmemory){
                std::cerr << "Error: Memory is empty\n";
                return false;
            }
            currentNumber = memory;

        } else if (command == "q"){
            break;
        } else {
            std::cerr << "Error: Unknown token " << command << "\n";
            return false;
        }
    }
    return true;
}

    
