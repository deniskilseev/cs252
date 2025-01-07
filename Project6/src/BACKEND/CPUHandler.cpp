// CPUHandler.cpp
#include "CPUCore.h"
#include "CPU.h"
#include "CPUHandler.h"

#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>

CPUHandler::CPUHandler() {
	int numCores = 0;
	std::string cpuName = "";
	std::ifstream cpuInfo("/proc/cpuinfo");
	assert(cpuInfo.is_open() && "Cpuinfo is either busy or does not exist\n");
	std::string line = "";
	int flag = 1;
	while (getline(cpuInfo, line)) {
		if (line.find("processor") != std::string::npos) {
			numCores ++;
		}
		if (flag && line.find("model name") != std::string::npos) {
			int index = line.find(":") + 2;
			cpuName.append(line.substr(index, line.size() - index - 1));
			flag = 0;
		}
	}

	cpuInfo.close();

	std::vector<CPUCore> cores(numCores);

	this->cpu = CPU(cpuName, numCores, cores);

	this->update();
	usleep(40000);
	this->update();
}

void CPUHandler::update() {
	std::string line = "";
	std::ifstream coreInfo("/proc/stat");
	assert(coreInfo.is_open() && "stat is either busy or does not exist\n");
	getline(coreInfo, line); // Skip the overall load on CPU.

	for (int coreID = 0; coreID < this->cpu.get_num_cores(); coreID++) {
		getline(coreInfo, line);
		int numberStatistics = 0;
		std::string number = "";
		unsigned long totalUptime = 0;
		unsigned long totalDowntime = 0;
		for (auto character : line) {
			if (character == ' ') {
				if (numberStatistics != 0) {
					unsigned long value = std::stoul(number, nullptr);
					if (numberStatistics == 4) {
						totalDowntime += value;
					}
					totalUptime += value;
				}
				number = "";
				numberStatistics ++;
			} else {
				number += character;
			}
		}
		double deltaDowntime = totalDowntime - this->cpu.get_core(coreID).get_downtime();
		double deltaUptime = totalUptime - this->cpu.get_core(coreID).get_uptime();
		this->cpu.update_core_uptime(coreID, totalUptime);
		this->cpu.update_core_downtime(coreID, totalDowntime);
		double load = 100.0 * (1 - deltaDowntime / deltaUptime);
		this->cpu.update_core_load(coreID, load);
	}

	coreInfo.close();

}
