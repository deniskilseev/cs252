// StorageHandler.cpp

#include "MountPoint.h"
#include "StorageHandler.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sys/statvfs.h>

std::vector<std::string> checkDevice(std::string line) {
	std::string device;
	std::string mountPoint;
	std::string encodingStandart;
	int deviceFlag = 1;
	int mountFlag = 1;
	int encodingFlag = 0;
	for (auto character : line) {
		if(encodingFlag) {
			break;
		}
		if (character == ' ' || character == '\t') {
			if (device.size() != 0) {
				deviceFlag = 0;
			}
			if (mountPoint.size() != 0) {
				mountFlag = 0;
			}
			if (encodingStandart.size() != 0) {
				encodingFlag = 1;
			}
		} else {
			if(deviceFlag) {
				device += character;
			} else if(mountFlag) {
				mountPoint += character;
			} else {
				encodingStandart += character;
			}
		}
	}
	if (device.find("/dev/") != std::string::npos) {
		return {device, mountPoint, encodingStandart};
	} else {
		return {};
	}
}

StorageHandler::StorageHandler() {
	std::ifstream mountInfo("/proc/mounts");

	assert(mountInfo.is_open() && "mounts is either busy or does not exist\n");
	std::string line = "";
	std::vector<std::string> devices(0);
	std::vector<std::string> mountPoints(0);
	std::vector<std::string> encodings(0);
	std::vector<unsigned long> sizes(0);
	std::vector<unsigned long> freeSpaces(0);
	std::vector<unsigned long> availableSpaces(0);
	int mountPointNumber = 0;

	while (getline(mountInfo, line)) {
		if(line.find("/dev/") != std::string::npos) {
			// Check whether that's a device or the /dev partition
			std::vector<std::string> info = checkDevice(line);
			if (info.size() != 0) {
				devices.push_back(info[0]);
				mountPoints.push_back(info[1]);
				encodings.push_back(info[2]);
				struct statvfs mountPoint;
				statvfs(info[1].c_str(), &mountPoint);
				sizes.push_back((unsigned long) ((mountPoint.f_blocks *
											mountPoint.f_frsize) / 1024));
				freeSpaces.push_back((unsigned long) ((mountPoint.f_bfree *
											mountPoint.f_bsize) / 1024));
				availableSpaces.push_back((unsigned long) ((mountPoint.f_bavail *
											mountPoint.f_bsize) / 1024));
				mountPointNumber ++;
			}
		}
	}

	mountInfo.close();

	for (int index = 0; index < mountPointNumber; index ++) {
		this->mountPoints.push_back(MountPoint(mountPoints[index],
												devices[index],
												encodings[index],
												sizes[index],
												freeSpaces[index],
												availableSpaces[index]));
	}
}

void StorageHandler::update() {
	// TODO: update the loads on the mount points and the devices if need be.
	std::ifstream mountInfo("/proc/mounts");

	assert(mountInfo.is_open() && "mounts is either busy or does not exits\n");
	std::string line = "";

	std::vector<int> isThere(this->mountPoints.size(), 0);

	while (getline(mountInfo, line)) {
		if (line.find("/dev/") != std::string::npos) {
			std::vector<std::string> info = checkDevice(line);
			if (info.size() == 0) {
				continue;
			}
			int flag = 1;
			for (int index = 0; index < this->mountPoints.size(); index ++) {
				if(this->mountPoints[index].get_device().compare(info[0]) == 0) {
					flag = 0;
					isThere[index] = 1;
					this->mountPoints[index].set_name(info[1]);
					this->mountPoints[index].set_encoding(info[2]);
					struct statvfs mountPoint;
					statvfs(info[1].c_str(), &mountPoint);
					this->mountPoints[index].set_size((unsigned long) ((mountPoint.f_blocks *
										mountPoint.f_frsize) / 1024));
					this->mountPoints[index].set_free_space((unsigned long)
									((mountPoint.f_bfree * mountPoint.f_bsize) / 1024));
					this->mountPoints[index].set_available_space((unsigned long)
									((mountPoint.f_bavail * mountPoint.f_bsize) / 1024));
				}
			}
			if (flag) {
				struct statvfs mountPoint;
				statvfs(info[1].c_str(), &mountPoint);
				unsigned long size = (unsigned long) ((mountPoint.f_blocks *
											mountPoint.f_frsize) / 1024);
				unsigned long freeSpace = (unsigned long) ((mountPoint.f_bfree *
											mountPoint.f_bsize) / 1024);
				unsigned long availableSpace = (unsigned long) ((mountPoint.f_bavail *
											mountPoint.f_bsize) / 1024);
				this->mountPoints.push_back(MountPoint(info[1], info[0], info[2],
											size, freeSpace, availableSpace));
				isThere.push_back(1);
			}
		}
	}
	int deletedNumber = 0;
	for (int index = 0; index < this->mountPoints.size(); index ++) {
		if (!isThere[index]) {
			this->mountPoints.erase(this->mountPoints.begin() + index - deletedNumber);
			deletedNumber ++;
		}
	}
}

void StorageHandler::print() {
	for (auto mount : this->mountPoints) {
		mount.print();
	}
}
