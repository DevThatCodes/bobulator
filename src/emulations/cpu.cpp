#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <cmath>
#include "cpu.hpp"

CPU::CPU(int programReg, int arithmeticReg, int architectureIn, int speedHZin, int coresIn, int TPCin) {
    optcode = {0,0,0,0};
    operand = {0,0,0,0};
    programRegs = programReg;
    architecture = architectureIn,
    speedHZ = speedHZin;
    cores = coresIn;
    TPC = TPCin;
    for (int i=0;i<programReg+arithmeticReg;i++) {
        cpuMem.push_back({0,0,0,0,0,0,0,0});
    }
}

int CPU::convertToInt(std::vector<int> convertStart) {
    int returnInt=0;
    for (int i=0;i<convertStart.size();i++) {
        if (convertStart[i] == 1) {
            returnInt += pow(2,(convertStart.size()-(i+1)));
        }
    }
    return returnInt;
}

std::vector<int> CPU::convertToBits(int convertStart, int length) {
    int convert = convertStart;
    int remainder = 0;
    std::vector<int> returnVector;
    for (int i=0;i<length;i++) {
        remainder = (int) convert % 2;  
        convert = convert / 2;
        returnVector.push_back(remainder);
        std::cout << remainder << "\n";
    }
    return returnVector;
}

std::vector<int> CPU::decodeOptcode(std::string optcodeIn) {
    std::vector<int> optcodeOut = {0,0,0,0};
    if (optcodeIn=="LREG") { optcodeOut = {0,0,0,1}; }
    if (optcodeIn=="RREG") { optcodeOut = {0,0,1,0}; }
    if (optcodeIn=="CREG") { optcodeOut = {0,0,1,1}; }
    if (optcodeIn=="ADD") { optcodeOut = {0,1,0,0}; }
    if (optcodeIn=="SUB") { optcodeOut = {0,1,0,1}; }
    if (optcodeIn=="MULT") { optcodeOut = {0,1,1,0}; }
    if (optcodeIn=="DIV") { optcodeOut = {0,1,1,1}; }
    if (optcodeIn=="JMP") { optcodeOut = {1,0,0,0}; }
    if (optcodeIn=="HALT") { optcodeOut = {1,0,0,1}; }
    if (optcodeIn=="PRT") { optcodeOut = {1,0,1,0}; }
    return optcodeOut;
}

std::vector<int> CPU::decodeOperand(std::string operandIn) {
    std::vector<int> operandOut = {0,0,0,0};
    for (int i=0;i<operandIn.size();i++) {
        std::cout << "operandIn[" << i << "]: " << operandIn[i] << "\n";
        if (operandIn[i]=='1') {
            operandOut[i] = 1;
        } else {
            operandOut[i] = 0;
        }
    }
    return operandOut;
}

int CPU::loadProgram(std::string program[], int programSize) {
    std::cout << "program size: " << programSize << "\n";
    if (programSize > programRegs) { 
        return 1;
    }
    
    for (int i=0;i<programSize;i++) {
        std::string token;
        std::string optcodeStr;
        std::string operandStr;
        std::cout << "lineSize: " << program[i].size() << "\n";
        for (int j=0;j<program[i].size();j++) {
            if (program[i].size()==4 && j==3) {
                optcodeStr = token + program[i][j];
                token = "";
            } else if (program[i][j] == ' ') {
                optcodeStr = token;
                token = "";
            } else {
                token += program[i][j];
            }
            if (j+1==program[i].size()) {
                operandStr = token;
                token = "";
            }           
        }
        optcode = decodeOptcode(optcodeStr);
        operand = decodeOperand(operandStr);
        std::cout << "cpuMem[" << i << "]: ";
        for (int j=0;j<8;j++) {
            if (j>=4) {
                cpuMem[i][j] = operand[j-4];
            } else {
                cpuMem[i][j] = optcode[j];
            }
            std::cout << cpuMem[i][j];
        }
        std::cout << "\n";
        std::cout << "optcode:";
        for (int j=0;j<optcode.size();j++) {
            std::cout << optcode[j];
        }
        std::cout << ", optcodeStr:" << optcodeStr << "\n";
        std::cout << "operand:";
        for (int j=0;j<operand.size();j++) {
            std::cout << operand[j];
        }
        std::cout << ", operandStr:" << operandStr << "\n";
    }   
    
    return 0;
}

int CPU::clearLoadedProgram() {
    std::vector<int> noBitsOn = {0,0,0,0,0,0,0,0};
    for (int i=0;i<programRegs;i++) {
        cpuMem[i] = noBitsOn;

    }

    return 0;
}

int CPU::emulate() {
    for (int i=0;i<1000;i++) {
        if (i==0) {
            if (pc >= programRegs) {
                std::cout << "programCounter reached end of available storage. clearing loaded programs!\n";
                pc = 0;
                clearLoadedProgram();
            }
            std::cout << pc << ":";
            for (int j=0;j<cpuMem[pc].size();j++) {
                std::cout << cpuMem[pc][j];
            }
            std::cout << "\n";
            if (runCommand(cpuMem[pc]) == 1) {
                std::cout << "Program has halted with exit code 1\n";
                pc = 0;
                clearLoadedProgram();
            }
            pc++;   
        }
        usleep(1000);
    }
    return 0;
}

int CPU::runCommand(std::vector<int> command) { // returns 0 if success, 1 if exited by program, 2 if failed
    std::vector<int> HALT = {1,0,0,1,0,0,0,0};
    std::vector<int> LREG = {0,0,0,1};
    std::vector<int> RREG = {0,0,1,0};
    std::vector<int> CREG = {0,0,1,1};
    std::vector<int> ADD = {0,1,0,0};
    std::vector<int> SUB = {0,1,0,1};
    std::vector<int> MULT = {0,1,1,0};
    std::vector<int> DIV = {0,1,1,1};
    std::vector<int> JMP = {1,0,0,0};
    std::vector<int> PRT = {1,0,1,0};
    std::vector<int> commandSectionConstructor;
    if (command==HALT) { return 1; } else {
        // splits command into optcode and operand
        for (int i=0;i<command.size();i++) {
            commandSectionConstructor.push_back(command[i]);
            if (i==3) {
                optcode = commandSectionConstructor;
                commandSectionConstructor.clear();
            } else if (i==7) {
                operand = commandSectionConstructor;
                commandSectionConstructor.clear();
            }
        }
        // run optcodes and operands
        if (optcode==LREG) {
            curArithReg = convertToInt(operand); 
            std::cout << "loaded register: " << curArithReg << " for arithmetic operations\n";
        } else if (optcode==RREG) {
            curReadReg = convertToInt(operand);
            std::cout << "loaded register: " << curReadReg << " for reading\n";
        } else if (optcode==CREG) {
            curArithReg = 0;
            curReadReg = 0;
        } else if (optcode==PRT) {
            std::cout << "prt: ";
            for (int i=0;i<cpuMem[programRegs+convertToInt(operand)].size();i++) {
                std::cout << cpuMem[programRegs+convertToInt(operand)][i];
            }
            std::cout << "\n";
        } else if (optcode==ADD) {
            int operandInt = convertToInt(operand);
            int curArithRegDat = convertToInt(cpuMem[programRegs+curArithReg]);
            std::cout << "adding " << operandInt << " to register: " << curArithReg << " that has a value of " << curArithRegDat << ", result: " << operandInt + curArithRegDat << "\n";
            std::vector<int> result = convertToBits(operandInt + curArithRegDat, 4);
            std::cout << "result in bits: ";
            for (int i=0;i<result.size();i++) {
                std::cout << result[i];
            }
            std::cout << "\n";
        } else { return 2; }
    }
    return 0;
}
