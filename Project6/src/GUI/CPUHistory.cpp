#include "CPUHistory.h"

#include "../BACKEND/CPU.h"
#include "../BACKEND/CPUCore.h"
#include "../BACKEND/CPUHandler.h"

#include <string>
#include <vector>
#include <iostream>

std::vector<double> cpuHistory() {
    std::vector<double> cpu_history;

    CPUHandler cpu_handler = CPUHandler();

    CPU cpu = cpu_handler.get_cpu();
    std::vector<CPUCore> cpu_cores = cpu.get_cores();
    for (size_t i = 0; i < 20 && i < cpu_cores.size(); ++i) {
        CPUCore core = cpu_cores[i];
        double load = core.get_load(); 
        cpu_history.push_back(load);
        /*
        cpu_history += "Load: " + std::to_string(core.get_load()) 
                       + ", Uptime: " + std::to_string(core.get_uptime()) 
                       + ", Downtime: " + std::to_string(core.get_downtime()) 
                       + "\n\n\n";
        */
       //std::cout << load << ", ";
    }
    //std::cout << "\n\n";
    return cpu_history;
}