#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include "Cursor.h"
#include "Process.h"
#include <vector>
#include <time.h>
#include <stdlib.h>

class Manager {
    private:
        static const int ON_HOLD_X_POS = 0;
        static const int QUEUED_X_POS = 42;
        static const int FINISHED_X_POS = 83;
        static const int STATE_POS = 110;
        static const int DELAY = 1000;
        std::vector<std::vector<Process>> batch;
        std::vector<Process> done;
        Process workingProcess;
        int globalCounter;
        int batchesCounter;
        int totalProcess;
        char state;
        bool pause;

        bool validateId(int auxInt, std::vector<int> &ids);
        bool isOperationValid(Process &auxProcess);
        void pop_front(std::vector<Process> &v);
        void printInProgress();
        void printFinished();
        void queueProcess();
        void doneProcess();
        void printQueued();
        void selectState(char character);
    public:
        Manager() { 
            pause = false;
            state = ' ';
            globalCounter = 0;
            workingProcess.setId(0);
            workingProcess.setTR(1);
            srand (time(NULL));
        }
        ~Manager() {}
        void printData();
        void getData();
        void createData();
};

#endif //MANAGER_H