#include "../BACKEND/Process.h"
#include "../BACKEND/ProcessHandler.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>

std::unordered_map<int, bool> seenInts;
std::unordered_map<int, Process> process_map;

std::string childStr(Process process_init, std::vector<Process> processes, int j) {
    //std::cout << process_init.get_pid() << "  :  " << seenInts[process_init.get_pid()] <<"\n ";
    std::string out = "";
    std::vector<unsigned long> children_processIDs = process_init.get_children_pids();
    //std::cout << children_processIDs.size() << " <- size,   ";
    for (int index = 0; index < children_processIDs.size(); index++) {
	Process process = process_map[children_processIDs[index]];

	if (seenInts[process.get_pid()]) continue;
        seenInts[process.get_pid()] = true;
	
	std::ostringstream formattedString;
	//std::cout << process.get_name() << "     ";
	formattedString << std::setw(4 * j) << "" << "Name: " << process.get_name() 
            << ",      Status: " << process.get_status() << ",      ID: " << std::to_string(process.get_pid()) 
            << ",      Memory Load: " << std::to_string(process.get_memory_load()) << "\n";
        std::string result = formattedString.str();

	out += result;
        out += childStr(process, processes, j + 1);
    }
    return out;
}

std::string showTreeProcessesGUI(ProcessHandler handler) {
    std::string process_info;

    seenInts = {};
    process_map = {};

    std::vector<Process> processes = handler.get_processes();

    for (int index = 0; index < processes.size(); index++) {
        process_map[processes[index].get_pid()] = processes[index];
    }

    std::cout << "num processes: " << processes.size();
    for (int index = 0; index < processes.size(); index++) {
        if (seenInts[processes[index].get_pid()]) continue;
        seenInts[processes[index].get_pid()] = true;
	
	Process process = processes[index];
        process_info += "Name: " + process.get_name() + 
                              ",      Status: " + process.get_status() +
                              ",      ID: " + std::to_string(process.get_pid()) +
                              ",      Memory Load: " + std::to_string(process.get_memory_load())
                              + "\n";
        //std::cout << index << " ,   num kids: " << process.get_children_pids().size() << "\n";
        if (process.get_children_pids().size() != 0) {
	  std::string child_str = childStr(process, processes, 1);
	  process_info += child_str;
          //std::cout << child_str << "\n";
	}
    }
    return process_info;
}
