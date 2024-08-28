#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include "emulations/cpu.hpp"
#include "headers/json.hpp"
using json = nlohmann::json;

json init(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "missing arguments!\n";
    } else {
        if (std::string(argv[1]) == "new") {
            if (argc != 7) {
                std::cout << "missing arguments for new vm generation!\n";
            } else {
                std::ofstream VM(std::string(argv[2]) + ".bobvm");
                VM << "{\n    \"name\": \""+std::string(argv[2])+"\",\n    \"cpu\": \""+std::string(argv[3])+"\",\n    \"ram\": \""+std::string(argv[4])+"\",\n    \"gpu\": \""+std::string(argv[6])+"\",\n    \"harddrive\": \""+std::string(argv[5])+"\"\n}\n";
                VM.close();
                std::ifstream VMf(std::string(argv[2]) + ".bobvm");
                json VMDat = json::parse(VMf);
                VMf.close();
                return VMDat;
            }

        } else if (std::string(argv[1]) == "run") {
            std::ifstream VM(argv[2]);
            json VMDat = json::parse(VM);
            VM.close();
            return VMDat;
        } else {
            std::cout << "invalid argument: " << argv[1] << "!\n";
        }
    }
    return {};
}

int main(int argc, char *argv[]) {
    json VMDat = init(argc, argv);
    std::cout << (std::string) VMDat["name"] << "'s specs\n";
    if ((std::filesystem::exists((std::string) VMDat["cpu"]) + std::filesystem::exists((std::string) VMDat["ram"]) + std::filesystem::exists((std::string) VMDat["gpu"]) + std::filesystem::exists((std::string) VMDat["harddrive"])) == 4) { // check if all parts are valid
        std::fstream cpu((std::string) VMDat["cpu"]);
        std::fstream ram((std::string) VMDat["ram"]);
        std::fstream gpu((std::string) VMDat["gpu"]);
        std::fstream harddrive((std::string) VMDat["harddrive"]);
        json cpuDat = json::parse(cpu);
        json ramDat = json::parse(ram);
        json gpuDat = json::parse(gpu);
        json harddriveDat = json::parse(harddrive);
        cpu.close();
        ram.close();
        gpu.close();
        harddrive.close();
        std::cout << "cpu: " << cpuDat["componentSpecs"]["name"] << "\n";
        std::cout << "ram: " << ramDat["componentSpecs"]["name"] << "\n";
        std::cout << "gpu: " << gpuDat["componentSpecs"]["name"] << "\n";
        std::cout << "harddrive: " << harddriveDat["componentSpecs"]["name"] << "\n";
        CPU VMCPU(cpuDat["componentSpecs"]["programReg"],cpuDat["componentSpecs"]["arithmeticReg"],cpuDat["componentSpecs"]["architecture"],cpuDat["componentSpecs"]["speedHZ"],cpuDat["componentSpecs"]["cores"],cpuDat["componentSpecs"]["threadsPerCore"]);
        VMCPU.loadProgram((std::string[4]){"LREG 0001",  "ADD 0001", "PRT 0001", "HALT"}, 4);
        while (true) {
            VMCPU.emulate();
        }
    }
    return 0;
}
