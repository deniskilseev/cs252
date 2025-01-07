// ProcessHandler.h
#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H

#include "Process.h"

#include <vector>

class ProcessHandler {
	private:
		std::vector<Process> processes;
	public:
	ProcessHandler(); // TODO: constructs each process

	std::vector<Process> get_processes() {
		return this->processes;
	}

	Process get_process(unsigned long pid) {
		for (auto process : this->processes) {
			if(process.get_pid() == pid) {
				return process;
			}
		}
		return Process();
	}

	void update(); // TODO: update each atribute of each process.
};

#endif
