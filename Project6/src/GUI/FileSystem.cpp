#include "FileSystem.h"

#include "../BACKEND/MountPoint.h"
#include "../BACKEND/StorageHandler.h"

std::string filesString() {
    std::string file_str;

    StorageHandler storage_handler = StorageHandler();
    std::vector<MountPoint> mount_points = storage_handler.get_mount_points();

    for (MountPoint& mount : mount_points) {
        file_str += "Name: " + mount.get_name()
               + ",   Device: " + mount.get_device()
               + ",   Encoding: " + mount.get_encoding() + "\n"
               + "Size: " + std::to_string(mount.get_size()) + " MB"
               + ",   Free Space: " + std::to_string(mount.get_free_space()) + " MB"
               + ",   Available Space: " + std::to_string(mount.get_available_space()) + " MB"
               + "\n\n";
    }

    return file_str;
}