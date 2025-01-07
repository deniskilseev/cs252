#include "../BACKEND/Process.h"
#include "../BACKEND/ProcessHandler.h"

#include <vector>
#include <string>
#include <iostream>

std::string showAllProcessesGUI(ProcessHandler handler) {
    std::vector<Process> processes = handler.get_processes();

    std::string process_info;

    //std::cout << "Num processes: " << processes.size() << "\n";

    for (int index = 0; index < processes.size(); index++) {
		Process process = processes[index];
        process_info += "Name: " + process.get_name() + 
                              ",      Status: " + process.get_status() +
                              ",      ID: " + std::to_string(process.get_pid()) +
                              ",      Memory Load: " + std::to_string(process.get_memory_load())
                              + "\n";
    }

    return process_info;
}
