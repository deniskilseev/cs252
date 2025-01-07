// MountPoint.h
#ifndef MOUNT_POINT_H
#define MOUNT_POINT_H

#include <string>

class MountPoint {
	private:
		std::string name;
		std::string device;
		std::string encoding;
		unsigned long size; // size of partition in MB;
		unsigned long freeSpace; // Free space of partition;
		unsigned long availableSpace; // Space available for an unpriveleged user;
	public:
		MountPoint() {
			this->name = "";
			this->device = "";
			this->encoding = "";
			this->size = 1;
			this->freeSpace = 0;
			this->availableSpace = 0;
		}

		MountPoint(std::string name, std::string device, std::string encoding, unsigned long size, unsigned long free, unsigned long available) {
			this->name = name;
			this->device = device;
			this->encoding = encoding;
			this->size = size;
			this->freeSpace = free;
			this->availableSpace = available;
		}

		std::string get_name() {
			return this->name;
		}

		void set_name(std::string name) {
			this->name = name;
		}

		std::string get_device() {
			return this->device;
		}

		std::string get_encoding() {
			return this->encoding;
		}

		void set_encoding(std::string encoding) {
			this->encoding = encoding;
		}

		unsigned long get_size() {
			return this->size;
		}

		void set_size(unsigned long size) {
			this->size = size;
		}

		unsigned long get_free_space() {
			return this->freeSpace;
		}

		void set_free_space(unsigned long free) {
			this->freeSpace = free;
		}

		unsigned long get_available_space() {
			return this->availableSpace;
		}

		void set_available_space(unsigned long available) {
			this->availableSpace = available;
		}

		void print();
};

#endif
