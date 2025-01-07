// CPUHandler.h
#ifndef CPUHANDLER_H
#define CPUHANDLER_H

#include "CPU.h"

class CPUHandler {
	private:
		CPU cpu;
	public:
        CPUHandler(); // Constructor for the CPU

        CPU get_cpu() {
            return this->cpu;
        }

        void update(); // Update the loads on each core

};

#endif
