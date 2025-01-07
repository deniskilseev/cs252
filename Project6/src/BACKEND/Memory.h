// Memory.h
#ifndef MEMORY_H
#define MEMORY_H

#include <cassert>

class Memory {
	private:
		unsigned long load; // Load in megabytes
		unsigned long capacity; // Capacity in megabytes
	public:
		Memory() {
			this->load = 0;
			this->capacity = 1;
		}

		Memory(unsigned long capacity, unsigned long load) {
			assert(load <= capacity && "Memory load cannot exceed it's capacity");
			this->load = load;
			this->capacity = capacity;
		}

		unsigned long get_load() {
			return this->load;
		}

		void set_load(unsigned long load) {
			if (load > this->capacity) {
				return ;
			}
			this->load = load;
		}

		unsigned long get_capacity() {
			return this->capacity;
		}

		void print(); // Function for testing
};
#endif
