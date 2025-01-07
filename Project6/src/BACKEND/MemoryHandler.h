//  MemoryHandler.h
#ifndef MEMORY_HANDLER_H
#define MEMORY_HANDLER_H
#include "Memory.h"

class MemoryHandler {
	private:
		Memory ram;
		Memory swap;
	public:

		MemoryHandler(); // Initialise swap and RAM

		Memory get_memory() {
			return this->ram;
		}

		Memory get_swap() {
			return this->swap;
		}

		void update(); // Update the loads on swap and RAM

		void print(); // Only used for tests
};
#endif
