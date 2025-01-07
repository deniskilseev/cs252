// CPU.h
#ifndef CPU_H
#define CPU_H
#include "CPUCore.h"

#include <cassert>
#include <string>
#include <vector>

class CPU {
	private:
		std::string cpuName;
		int numCores;
		std::vector<CPUCore> cores;
	public:
		CPU() {
			this->cpuName = "";
			this->numCores = 0;
		}

		CPU(std::string cpuName, int numCores, std::vector<CPUCore> cores) {
			assert(cores.size() == numCores && "Number of Cores in differnet from vector length");
			this->cpuName = cpuName;
			this->numCores = numCores;
			this->cores = cores;
		}

		std::vector<CPUCore> get_cores() {
			return this->cores;
		}

		CPUCore get_core(int index) {
			if (index >= cores.size()) {
				return CPUCore();
			}
			return this->cores[index];
		}

		std::string get_name() {
			return this->cpuName;
		}

		int get_num_cores() {
			return this->numCores;
		}

		void update_core_load(int index, double load) {
			this->cores[index].set_load(load);
		}

		void update_core_uptime(int index, unsigned long uptime) {
			this->cores[index].set_uptime(uptime);
		}

		void update_core_downtime(int index, unsigned long downtime) {
			this->cores[index].set_downtime(downtime);
		}

		void set_loads(std::vector<unsigned long> coreLoads) {
			if (coreLoads.size() != numCores) {
				return ;
			}
			for (int i = 0; i < this->get_num_cores(); i++) {
				this->cores[i].set_load(coreLoads[i]);
			}
		}

		void print();
};
#endif
