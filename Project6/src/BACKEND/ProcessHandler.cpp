// ProcessHandler.cpp
#include "ProcessHandler.h"
#include "Process.h"


#include <filesystem>
#include <iostream>
#include <string>

// Implement the undefined methods in the header file

ProcessHandler::ProcessHandler() {
	std::string process_directory = "/proc";
	for(const auto &file: std::filesystem::directory_iterator(process_directory)) {
		std::string current_file = file.path();
		std::string word = "";
		for (int index = 6; index < current_file.size(); index++) { // Skipping the /proc/ in the beginning
			if (current_file[index] >= '0' && current_file[index] <= '9') {
				word += current_file[index];
			} else {
				word = "";
				break;
			}
		}
		if (word.size() != 0) { // A process directory
			this->processes.push_back(Process(std::stoul(word)));
		}
	}
	this->update();
    // Constructor implementation
}

void ProcessHandler::update() {
	// TODO: update the list of processes and call update_self() for each of the process.
	std::string process_directory = "/proc";
	std::vector<int> is_there(this->processes.size(), 1);
	for(const auto &file: std::filesystem::directory_iterator(process_directory)) {
		std::string current_file = file.path();
		std::string word = "";
		for (int index = 6; index < current_file.size(); index++) { // Skippin the /proc/ in the beginning
			if (current_file[index] >= '0' && current_file[index] <= '9') {
				word += current_file[index];
			} else {
				word = "";
				break;
			}
		}
		if (word.size() != 0) { // A process directory
			int flag = 1;
			int currentPid = std::stoul(word);
			for (int process_index = 0; process_index < this->processes.size(); process_index ++) {
				if (this->processes[process_index].get_pid() == currentPid) {
					flag = 0;
					is_there[process_index] = 0;
					break;
				}
			}
			if (flag) {
				this->processes.push_back(Process(currentPid));
				is_there.push_back(0);
			}
		}
	}
	// Remove the processes that are not there
	int amountRemoved = 0;
	for (int index = 0; index < is_there.size(); index ++) {
		if (is_there[index]) {
			this->processes.erase(this->processes.begin() + index -amountRemoved);
			amountRemoved += 1;
		}
	}
	// Update each process.

	for (int index = 0; index < this->processes.size(); index ++) {
		this->processes[index].update_self();
		this->processes[index].free_child_pids();
	}

	for (int index = 0; index < this->processes.size(); index ++) {
		for (int parentIndex = 0; parentIndex < this->processes.size(); parentIndex ++) {
			if (this->processes[parentIndex].get_pid() == this->processes[index].get_parent_pid()) {
				this->processes[parentIndex].push_child_process(this->processes[index].get_pid());
				break;
			}
		}
	}
}
