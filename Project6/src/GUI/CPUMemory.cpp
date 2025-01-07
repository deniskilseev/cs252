#include "CPUHistory.h"

#include "../BACKEND/Memory.h"
#include "../BACKEND/MemoryHandler.h"

#include <string>
#include <vector>

std::vector<unsigned long> cpuMemory() {
    std::vector<unsigned long> cpu_memory;

    MemoryHandler memory_handler = MemoryHandler();
    Memory ram = memory_handler.get_memory();
    Memory swap = memory_handler.get_swap();

    cpu_memory.push_back(ram.get_load());
    cpu_memory.push_back(swap.get_load());

    /*
    cpu_memory += "RAM:       Load: " + std::to_string(ram.get_load()) 
                       + ", Capacity: " + std::to_string(ram.get_capacity()) 
                       + "\n";

    cpu_memory += "SWAP:       Load: " + std::to_string(swap.get_load()) 
                       + ", Capacity: " + std::to_string(swap.get_capacity()) 
                       + "\n\n\n";
    */
    return cpu_memory;
}