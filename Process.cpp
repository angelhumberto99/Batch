#include "Process.h"
#include <iostream>

bool Process::validateOperation(const std::string &value) {
    const std::regex op("-?[0-9]+[[:space:]]*[+][[:space:]]*[0-9]+");
    const std::regex op2("-?[0-9]+[[:space:]]*-[[:space:]]*[0-9]+");
    const std::regex op3("-?[0-9]+[[:space:]]*[/|%|*][[:space:]]*-?[0-9]+");
    return std::regex_match(value, op) || std::regex_match(value, op2)
        || std::regex_match(value, op3);
}

// setters
void Process::setProgrammerName(const std::string &value) {programmerName = value; }

void Process::setEMT(const int value) { EMT = value; }

void Process::setId(const int value) { id = value; }

void Process::setTT(int value) { TT = value; }

void Process::setTR(int value) { TR = value; }

void Process::setOperation(const std::string &value) {
    const std::regex num("[0-9]");
    const std::regex op("[+|*|/|%]");
    const std::regex op2("-");


    std::string num1 = "", 
                num2 = "", 
                oper = "", 
                aux = "";

    int k = 0;

    if (value[0] == '-') {
        k = 1;
        num1 = "-";
    }
    for (int i(k); i < value.length(); i++) { 
        aux = value[i];
        if (std::regex_match(aux, num) && oper == "")
            num1 += aux;
        else if ((oper == "/" || oper == "%" || oper == "*") && value[i] == '-')
            num2 += "-";
        else if (std::regex_match(aux, num))
            num2 += aux;
        else if (std::regex_match(aux, op) || std::regex_match(aux, op2))
            oper = aux;
        else
            continue;
    }

    // setteamos los valores
    if (num1[0] == '-') {
        aux = num1.substr(1);
        firstOperand = (atoi(aux.c_str())) * -1;
    }
    else
        firstOperand = atoi(num1.c_str());

    if (num2[0] == '-') {
        aux = num2.substr(1);
        secondOperand = (atoi(aux.c_str())) * -1;
    }
    else
        secondOperand = atoi(num2.c_str());

    operationSymbol = oper[0];
}


// getters
const std::string Process::getProgrammerName() const { return programmerName; }

const char Process::getOperationSymbol() const { return operationSymbol; }

const int Process::getSecondOperand() const { return secondOperand; }

const int Process::getEMT() const { return EMT; }

const int Process::getId() const { return id; }

const int Process::getTT() const { return TT; }

const int Process::getTR() const { return TR; }

const std::string Process::getOperation() const { 
    return std::to_string(firstOperand) 
            + operationSymbol 
            + std::to_string(secondOperand); 
}

const std::string Process::getResult() const {
    if (TR != 0)
        return "Error";
    switch(operationSymbol) { 
        case '+': return std::to_string(firstOperand + secondOperand);
        case '-': return std::to_string(firstOperand - secondOperand);
        case '*': return std::to_string(firstOperand * secondOperand);
        case '/': return std::to_string(firstOperand / secondOperand);
        case '%': return std::to_string(firstOperand % secondOperand);
        default: std::cout << "invalid operator" << std::endl; break;
    }
}