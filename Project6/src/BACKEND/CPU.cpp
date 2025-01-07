#include "CPU.h"

#include <iostream>

void CPU::print() {
	std::cout << "CPU name: " << this->cpuName << "\n";
	for (int i = 0; i < this->get_num_cores(); i++) {
		std::cout << "Core #" << i << " load: ";
		this->get_core(i).print();
	}
}
