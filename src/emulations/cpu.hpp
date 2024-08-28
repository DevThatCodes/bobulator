#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <cmath>
#ifndef CPUEMULATOR
#define CPUEMULATOR

class CPU {
    public:
        CPU(int programReg, int arithmeticReg, int architectureIn, int speedHZin, int coresIn, int TPCin);
        int loadProgram(std::string program[], int programSize);
        int emulate();
        int clearLoadedProgram();
        std::vector<int> decodeOptcode(std::string optcode);
        std::vector<int> decodeOperand(std::string operand);
        int convertToInt(std::vector<int> convertStart);
        std::vector<int> convertToBits(int convertStart, int length);
        int runCommand(std::vector<int> command);
        std::vector<std::vector<int>> cpuMem;

    private:
        int programRegs;
        int architecture;
        int speedHZ;
        int cores;
        int TPC;
        int pc=0;
        int curReadReg = 0;
        int curArithReg = 0;
        std::vector<int> optcode;
        std::vector<int> operand;
        
};

#endif
