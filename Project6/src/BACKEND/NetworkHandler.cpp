// NetworkHandler.cpp

#include "NetworkHandler.h"
#include "Network.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <utility>


NetworkHandler::NetworkHandler() {
	this->net = Network();
	update();
}

std::pair<unsigned long, unsigned long> getBytes() {
	std::ifstream networkInfo("/proc/net/dev");
	assert(networkInfo.is_open() && "Network is either busy or does not exits\n");
	std::string line = "";
	getline(networkInfo, line);
	getline(networkInfo, line); // Skip the names of the columns
	unsigned long totalDownload = 0;
	unsigned long totalUpload = 0;
	while (getline(networkInfo, line)) {
		int parameterNumber = 0;
		std::string word = "";
		for(auto character : line) {
			if (character == ' ' || character == '\t') {
				if(word.size() != 0) {
					if (parameterNumber == 1) {
						totalDownload += std::stoul(word);
					} else if (parameterNumber == 9) {
						totalUpload += std::stoul(word);
					}
					word = "";
					parameterNumber ++;
				}
			} else {
				word += character;
			}
		}
	}
	networkInfo.close();
	return std::make_pair(totalDownload, totalUpload);
}

void NetworkHandler::update() { // This kind of works but kind of doesn't because of the chaching of ifstream
	std::chrono::time_point<std::chrono::system_clock> first_read =
							std::chrono::system_clock::now();
	std::pair<unsigned long, unsigned long> first_bytes = getBytes();
	usleep(1000);
	std::chrono::time_point<std::chrono::system_clock> second_read =
							std::chrono::system_clock::now();
	std::pair<unsigned long, unsigned long> second_bytes = getBytes();
	double time = ((std::chrono::duration_cast<std::chrono::microseconds>) (second_read - first_read)).count();
	this->net.set_download_speed((unsigned long)
							(((double) ((second_bytes.first - first_bytes.first) * 1000) / time)));
	this->net.set_upload_speed((unsigned long)
							(((((double) second_bytes.second - first_bytes.second) * 1000) / time)));
}
