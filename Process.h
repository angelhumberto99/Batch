#ifndef PROCESS_H
#define PROCESS_H 

#include <string>
#include <regex>
#include <stdlib.h>

class Process{
    private:
        // miembros de la clase
        std::string programmerName;
        char operationSymbol;
        int secondOperand;
        int firstOperand;
        int EMT;
        int id;
        int TT;
        int TR;

    public:
        Process() { 
            TT = 0; 
            programmerName = "";
        }
        ~Process() {}

        bool validateOperation(const std::string &value);

        // setters
        void setProgrammerName(const std::string &value);
        void setOperation(const std::string &value);
        void setEMT(int value);
        void setId(int value);
        void setTT(int value);
        void setTR(int value);
        
        // getters
        const std::string getProgrammerName() const;
        const std::string getOperation() const;
        const char getOperationSymbol() const;
        const int getSecondOperand() const;
        const std::string getResult() const;
        const int getEMT() const;
        const int getId() const;
        const int getTT() const;
        const int getTR() const;

        // sobrecarga de operador
        const Process& operator=(const Process &other) {
            this->programmerName = other.programmerName;
            this->operationSymbol = other.operationSymbol;
            this->secondOperand = other.secondOperand;
            this->firstOperand = other.firstOperand;
            this->EMT = other.EMT;
            this->id = other.id;
            this->TT = other.TT;
            this->TR = other.TR;
            return *this;
        }
};

#endif //PROCESS_H