#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#include "../BACKEND/Memory.h"
#include "../BACKEND/MemoryHandler.h"
#include "../BACKEND/CPU.h"
#include "../BACKEND/CPUHandler.h"
#include "../BACKEND/StorageHandler.h"
#include "../BACKEND/MountPoint.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>


std::string getSystemInfo() {
    std::stringstream infoStream;

    infoStream << "Basic System Information:\n";

    struct utsname unameData;
    if (uname(&unameData) == 0) {
        infoStream << "  OS Name: " << unameData.sysname << "\n";
        infoStream << "  OS Release: " << unameData.release << "\n";
        infoStream << "  Kernel Version: " << unameData.version << "\n";
    } else {
        infoStream << "Error: Unable to retrieve OS information." << "\n";
    }

    //amount of memory is ram has been done in CPUMemory.cpp
    MemoryHandler memory_handler = MemoryHandler();
    Memory ram = memory_handler.get_memory();
    infoStream << "  RAM Memory: " << ram.get_capacity() << " MB\n";

    //processor name -> CPU.h getname()
    CPUHandler cpu_handler = CPUHandler();
    CPU cpu = cpu_handler.get_cpu();
    infoStream << "  Processor Version: " << cpu.get_name() << "\n";

    //disk storage -> mount point with name=="/", get availableSpace
    StorageHandler storage_handler = StorageHandler();
    std::vector<MountPoint> mount_points =  storage_handler.get_mount_points();
    for (MountPoint& mountPoint : mount_points) {
        if (mountPoint.get_name() == "/") {
            unsigned long availableSpace = mountPoint.get_available_space();
            infoStream << "  Disk Storage: " << std::to_string(availableSpace) << " MB";
        }
    }

    return infoStream.str();
}
