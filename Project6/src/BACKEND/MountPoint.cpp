// MountPoint.cpp
#include "MountPoint.h"

#include <iostream>

void MountPoint::print() {
	std::cout << "Device " << this->device << " is mounted on "
		<< this->name << " with encryption " << this->encoding <<  " with size of " << this->size << ", free space of " << this->freeSpace << ", and avalible space of " <<
		this->availableSpace << " megabytes." << "\n";
}
