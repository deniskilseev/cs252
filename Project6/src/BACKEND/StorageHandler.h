// StorageHandler.h
#ifndef STORAGE_HANDLER_H
#define STORAGE_HANDLER_H

#include "MountPoint.h"

#include<vector>

class StorageHandler {
	private:
		std::vector<MountPoint> mountPoints;
	public:
		StorageHandler();

		std::vector<MountPoint> get_mount_points() {
			return this->mountPoints;
		}

		void update();

		void print();
};

#endif
