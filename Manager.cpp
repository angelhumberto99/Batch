#include "Manager.h"

bool Manager::validateId(int auxInt, std::vector<int> &ids) {
    if(auxInt <= 0)
        return false;
    for(int i(0); i<ids.size(); i++){
        if (ids[i] == auxInt)
            return false;  
    }
    return true;
}

void Manager::pop_front(std::vector<Process> &v) {
    if (!v.empty()) {
        v.erase(v.begin());
    }
}

void Manager::queueProcess() {
    if (batch[0].empty()){
        batch.erase(batch.begin());
        batchesCounter--;
        workingProcess.setId(0);
        workingProcess.setTR(1);
        return;
    }
    if (!batch.empty())
        workingProcess = batch[0][0];
    else
        return;
    pop_front(batch[0]);
}

void Manager::doneProcess() {
    if (workingProcess.getId())
        done.push_back(workingProcess);
}

void Manager::selectState(char character) {
    state = character;
    switch (character) {
        case 'i':
            if(!pause) {
                batch[0].push_back(workingProcess);
                workingProcess.setId(0);
                workingProcess.setTR(1);
                break;
            }
        case 'e': 
            if(!pause) {
                doneProcess(); 
                queueProcess(); 
                break;
            }
        case 'p': 
            pause = true;
            CLEAR; 
            break;
        case 'c': 
            pause = false;
            CLEAR; 
            break;
    }
}

void Manager::printData() {
    batchesCounter = batch.size() - 1; // numero de lotes pendientes
    do{
        if (KBHIT())
            selectState(GETCH());
        if (!pause) {
            CLEAR;
            globalCounter++;
        }
        printInProgress();
        printQueued();
        printFinished();
        if (!pause) {
            workingProcess.setTT(workingProcess.getTT()+1);
            workingProcess.setTR(workingProcess.getTR()-1);
            if(workingProcess.getTR() == 0){
                doneProcess();
                queueProcess();
            }
        }
        SLEEP(DELAY);
    }while(!batch.empty());
        CLEAR;
        printQueued();
        printInProgress();
        printFinished();
}

// lote en espera
void Manager::printQueued() {
    if (batchesCounter > 0){
        GOTO_XY(ON_HOLD_X_POS, 1);
        std::cout << "No. Lotes pendientes: " << batchesCounter;
    }
    else{
        GOTO_XY(ON_HOLD_X_POS, 1);
        std::cout << "No. Lotes pendientes: 0";
    }
    GOTO_XY(QUEUED_X_POS-10, 1);
    std::cout << "Contador Global: " << globalCounter;
    GOTO_XY(FINISHED_X_POS-25, 1);
    std::cout << "Procesos totales: " << totalProcess;
    GOTO_XY(FINISHED_X_POS+2, 1);
    if (!batch.empty())
        std::cout << "Estado: " << (pause?"pausado":"en ejecucion");
    else
        std::cout << "Estado: Terminado";
    GOTO_XY(STATE_POS, 1);
    std::cout << "Entrada: " << state;
    GOTO_XY(ON_HOLD_X_POS, 3);
    if (batch.empty() || batch[0].empty()) {
        std::cout << "Lote en ejecucion" << std::endl;
        std::cout << ".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-" << std::endl;
        return;
    }
    std::cout << "Lote en ejecucion" << std::endl; 
    for(int i(0); i<batch[0].size(); i++) {
        std::cout << ".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-" << std::endl
                  << "ID: " << batch[0][i].getId() << std::endl
                  << "TME: " << batch[0][i].getEMT() << std::endl
                  << "TT: " << batch[0][i].getTT() << std::endl;
    }
    
}

// proceso en ejecución
void Manager::printInProgress() {
    if (batch.empty() || workingProcess.getId() == 0) {
        GOTO_XY(QUEUED_X_POS, 3);
        std::cout << "Proceso en ejecucion";
        GOTO_XY(QUEUED_X_POS, 4);
        std::cout << ".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-";
        return;
    }
    GOTO_XY(QUEUED_X_POS, 3);
    std::cout << "Proceso en ejecucion";
    GOTO_XY(QUEUED_X_POS, 4);
    std::cout << ".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-";
    GOTO_XY(QUEUED_X_POS, 5);
    std::cout << "ID: " << workingProcess.getId();
    GOTO_XY(QUEUED_X_POS, 6);
    std::cout << "Operacion: " << workingProcess.getOperation();
    GOTO_XY(QUEUED_X_POS, 7);
    std::cout << "TME: " << workingProcess.getEMT();
    GOTO_XY(QUEUED_X_POS, 8);
    std::cout << "TT: " << workingProcess.getTT();
    GOTO_XY(QUEUED_X_POS, 9);
    std::cout << "TR: " << workingProcess.getTR();
}

// procesos finalizados
void Manager::printFinished() {
    int pos = 3;
    int batCount = 1;
    GOTO_XY(FINISHED_X_POS, pos);
    std::cout << "Finalizados";
    if (done.empty()) {
        GOTO_XY(FINISHED_X_POS, ++pos);
        std::cout << ".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-";
    }
    for(int i(0); i<done.size(); i++) {
        if (i % 5 == 0) {
            GOTO_XY(FINISHED_X_POS, ++pos);
            std::cout << ".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-";
            GOTO_XY(FINISHED_X_POS, ++pos);
            std::cout << "Lote: " << batCount;
            batCount++;
        }
        GOTO_XY(FINISHED_X_POS, ++pos);
        std::cout << ".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-";
        GOTO_XY(FINISHED_X_POS, ++pos);
        std::cout << "ID: " << done[i].getId();
        GOTO_XY(FINISHED_X_POS, ++pos);
        std::cout << "Operacion: " << done[i].getOperation();
        GOTO_XY(FINISHED_X_POS, ++pos);
        std::cout << "Resultado: " << done[i].getResult();
    }
}

bool Manager::isOperationValid(Process &auxProcess) {
    char oper = auxProcess.getOperationSymbol();
    int num2 = auxProcess.getSecondOperand();
    if ((oper == '%' || oper == '/') && num2 == 0)
        return false;
    return true;
}

void Manager::getData() {
    Process auxProcess;
    std::vector<Process> auxVec;
    int processQuantity = 0;
    std::string auxString = "";
    int auxInt = 0;
    std::vector<int> ids;

    // pedimos la cantidad de procesos
    std::cout << "Ingrese la cantidad de procesos que desea: ";
    std::cin >> processQuantity;
    std::cin.ignore();
    totalProcess = processQuantity;

    do {
        CLEAR;
        std::cout << "ingrese los siguientes datos" << std::endl

        // ingresamos el nombre del programador
        << "Nombre del programador: "; 
        getline(std::cin, auxString);
        auxProcess.setProgrammerName(auxString);

        // ingresamos un id valido
        do{
            std::cout << "ID: ";
            std::cin >> auxInt;
        }while(!validateId(auxInt, ids));
        auxProcess.setId(auxInt);
        ids.push_back(auxInt);
        std::cin.ignore();

        // ingresamos una operación valida
        bool band1 = false, band2 = false;
        do{
            std::cout << "Operacion: ";
            getline(std::cin, auxString);
            // validamos que la operación sea correcta
            band1 = auxProcess.validateOperation(auxString);
            if(band1){
                auxProcess.setOperation(auxString);
                band2 = isOperationValid(auxProcess);
            }
        }while(!band2);

        // ingresamos el tiempo máximo estimado
        do{
            std::cout << "Tiempo maximo estimado: ";
            std::cin >> auxInt;
        }while(auxInt <= 0);
        auxProcess.setEMT(auxInt);
        auxProcess.setTR(auxInt);
        std::cin.ignore();

        // ingresamos un proceso al "lote"
        auxVec.push_back(auxProcess);

        // si el lote se completa, entonces lo añadimos a la "cola"
        if (auxVec.size() == 5) {
            batch.push_back(auxVec);
            auxVec.clear();
        }
        processQuantity--;
    } while(processQuantity != 0);

    if(!auxVec.empty())
        batch.push_back(auxVec);
}

void Manager::createData() {
    Process auxProcess;
    std::vector<Process> auxVec;
    int processQuantity = 0;
    std::string auxString = "";
    int auxInt = 0;
    int uniqueKey = 1;
    std::string operands  = "+-*/%"; // operandos disponibles

    // pedimos la cantidad de procesos
    std::cout << "Ingrese la cantidad de procesos que desea: ";
    std::cin >> processQuantity;
    std::cin.ignore();
    totalProcess = processQuantity;

    do {
        // ingresamos un id valido
        auxProcess.setId(uniqueKey);
        // el id será consecutivo
        uniqueKey++;

        // ingresamos una operación valida
        bool band1 = false, band2 = false;
        do{
            auxString = "";
            auxString += std::to_string(rand()%100); // random de 0 - 99
            auxString += operands[rand()%5]; // random de 0-4
            auxString += std::to_string(rand()%100); // random de 0 - 99
            // validamos que la operación sea correcta
            band1 = auxProcess.validateOperation(auxString);
            if(band1){
                auxProcess.setOperation(auxString);
                band2 = isOperationValid(auxProcess);
            }
        }while(!band2);

        // ingresamos el tiempo máximo estimado
        auxInt = 6 + (rand() % 10); // random de 6 - 15
        auxProcess.setEMT(auxInt);
        auxProcess.setTR(auxInt);

        // ingresamos un proceso al "lote"
        auxVec.push_back(auxProcess);

        // si el lote se completa, entonces lo añadimos a la "cola"
        if (auxVec.size() == 5) {
            batch.push_back(auxVec);
            auxVec.clear();
        }
        processQuantity--;
    } while(processQuantity != 0);

    if(!auxVec.empty())
        batch.push_back(auxVec);
}