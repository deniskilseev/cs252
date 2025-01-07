// MemoryHandler.cpp
#include "Memory.h"
#include "MemoryHandler.h"

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>

unsigned long parseNumber(std::string line) {
	std::string number = "";
	for (auto character : line) {
		if (character <= '9' && character >= '0') {
			number += character;
		}
	}
	return std::stoul(number);
}

MemoryHandler::MemoryHandler() {
	std::ifstream memoryInfo("/proc/meminfo");

	assert(memoryInfo.is_open() && "memoryinfo is either busy or does not exist\n");

	unsigned long ramCapacity = 0; //The capacity and loads are in megabytes
	unsigned long ramLoad = 0;

	unsigned long swapCapacity = 0;
	int swapCapacityFlag = 1;
	unsigned long swapLoad = 0;
	int swapLoadFlag = 1;

	std::string line = "";
	while(getline(memoryInfo, line)) {
		if (ramCapacity == 0 && line.find("MemTotal:") != std::string::npos) { // Parse the RAM capacity
			ramCapacity = parseNumber(line) / 1024; // Since the capacity in the file is in kB
		}
		if (ramLoad == 0 && line.find("MemAvailable:") != std::string::npos) {
			ramLoad = ramCapacity - (parseNumber(line) / 1024);
		}
		if (swapCapacityFlag && line.find("SwapTotal:") != std::string::npos) {
			swapCapacity = parseNumber(line) / 1024;
			swapCapacityFlag = 0;
		}
		if (swapLoadFlag && line.find("SwapFree:") != std::string::npos) {
			swapLoad = swapCapacity - (parseNumber(line) / 1024);
			swapLoadFlag = 0;
		}
	}
	memoryInfo.close();
	this->ram = Memory(ramCapacity, ramLoad);
	this->swap = Memory(swapCapacity, swapLoad);
}

void MemoryHandler::update() {
	std::ifstream memoryInfo("/proc/meminfo");

	assert(memoryInfo.is_open() && "memoryinfo is either busy or does not exist\n");

	std::string line = "";

	int ramFlag = 1;
	int swapFlag = 1;

	while(getline(memoryInfo, line)) {
		if (ramFlag && line.find("MemAvailable:") != std::string::npos) {
			this->ram.set_load(this->ram.get_capacity() - (parseNumber(line) / 1024));
			ramFlag = 0;
		}
		if (swapFlag && line.find("SwapFree") != std::string::npos) {
			this->swap.set_load(this->swap.get_capacity() - (parseNumber(line) / 1024));
			swapFlag = 0;
		}
	}

	memoryInfo.close();
}

void MemoryHandler::print() {
	std::cout << "RAM usage:\n";
	this->ram.print();
	std::cout << "SWAP usage:\n";
	this->swap.print();
}
