// Network.cpp

#include "Network.h"

#include <iostream>

void Network::print() {
	std::cout << "Download speed: " << this->downloadSpeed << " Upload speed: " << this->uploadSpeed << "\n";
}
