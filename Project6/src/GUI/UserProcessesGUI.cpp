#include "../BACKEND/Process.h"
#include "../BACKEND/ProcessHandler.h"

#include <vector>
#include <string>
#include <unistd.h>

std::string showUserProcessesGUI(ProcessHandler handler) {
    std::string process_info;

    std::vector<Process> processes = handler.get_processes();

    for (int index = 0; index < processes.size(); index++) {
        uid_t uid = geteuid();
        if (uid != processes[index].get_user_id()) continue;
		Process process = processes[index];
        process_info += "Name: " + process.get_name() + 
                              ",       Status: " + process.get_status() +
                              ",       ID: " + std::to_string(process.get_pid()) +
                              ",       Memory Load: " + std::to_string(process.get_memory_load())
                              + "\n";
    }

    return process_info;
}
