// Process.cpp

#include "Process.h"

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <pwd.h>

// Implement the TODOs in the header file

std::vector<std::string> split_string(std::string line)
{
    std::stringstream ss(line);
    std::string word;
    std::vector<std::string> returnStringVector = std::vector<std::string>();

    while (ss >> word)
    {
        returnStringVector.push_back(word);
    }

    return returnStringVector;
}

std::vector<std::string> split_string(std::string input, char separator)
{

    std::vector<std::string> tokens;
    std::istringstream ss(input);
    std::string token;

    while (std::getline(ss, token, separator))
    {
        tokens.push_back(token);
    }

    return tokens;
}

void parse_permissions(std::string permissions, MemoryMap &memoryMap)
{
    std::vector<int> permissionsVector = std::vector<int>();
    for (int idx = 0; idx < 4; idx++)
    {
        char const current = permissions.at(idx);
        if (current == '-')
        {
            permissionsVector.push_back(0);
        }
        else
        {
            permissionsVector.push_back(1);
        }
    }
    memoryMap.set_flags(permissionsVector);
    return;
}

void populate_fields_first_line(std::vector<std::string> splitVector, MemoryMap &memoryMap)
{
    for (int idx = 0; idx < 5; idx++)
    {
        std::string currentString = splitVector.at(idx).c_str();
        switch (idx)
        {
        case 0:
        {
            std::vector<std::string> startEndVMVector = split_string(currentString, '-');
            memoryMap.set_vm_start(startEndVMVector.at(0));
            memoryMap.set_vm_end(startEndVMVector.at(1));
            break;
        }
        case 1:
        {
            parse_permissions(currentString, memoryMap);
            break;
        }
        case 2:
        {
            memoryMap.set_vm_offset(currentString);
            break;
        }
        case 3:
        {
            memoryMap.set_io_device(currentString);
            break;
        }
        case 4:
        {
            memoryMap.set_inode(std::stoul(currentString));
            break;
        }
        }
    }
}

void populate_fields(int lineNumber, std::string line, MemoryMap &memoryMap)
{
    int neededLinesArray[] = {0, 1, 6, 7, 8, 9};
    for (auto neededLine : neededLinesArray)
    {
        if (lineNumber == neededLine)
        {
            std::vector<std::string> splitVector = split_string(line);
            switch (lineNumber)
            {
            case 0:
            {
                populate_fields_first_line(splitVector, memoryMap);
                if (splitVector.size() == 6)
                {
                    memoryMap.set_filename(splitVector.at(5));
                }
                break;
            }
            case 1:
            {
                memoryMap.set_vm_size(
                    std::stoul(splitVector.at(1)));
                break;
            }
            case 6:
            {
                memoryMap.set_shared_clean(
                    std::stoul(splitVector.at(1)));
                break;
            }
            case 7:
            {
                memoryMap.set_shared_dirty(
                    std::stoul(splitVector.at(1)));
                break;
            }
            case 8:
            {
                memoryMap.set_private_clean(
                    std::stoul(splitVector.at(1)));
                break;
            }
            case 9:
            {
                memoryMap.set_private_dirty(
                    std::stoul(splitVector.at(1)));
                break;
            }
            }
        }
    }
}

/*
 * Function returns all memory maps for a given instance of Process().
 *
 *
 * */
std::vector<MemoryMap> Process::list_memory_maps()
{

    int pid = this->get_pid();

    std::string pathToMaps = "/proc/" + std::to_string(pid) + "/smaps";

    std::ifstream mapsFile(pathToMaps);

    std::vector<MemoryMap> returnMapsVector = std::vector<MemoryMap>(0);

    if (mapsFile.is_open())
    {
        while (mapsFile.peek() != EOF)
        {
            // There are exactly 24 lines per memory maps
            MemoryMap currentMemoryMap = MemoryMap();
            std::string readLine;
            for (int line = 0; line < 24; line++)
            {
                std::getline(mapsFile, readLine);
                populate_fields(line, readLine, currentMemoryMap);
            }
            returnMapsVector.push_back(currentMemoryMap);
        }
    }
    mapsFile.close();
    return returnMapsVector;
}

void Process::stop_process()
{
    pid_t pid = this->process_id;

    int killing = kill(pid, SIGSTOP);
    if (killing == 0)
    {
        std::cout << "Stopped process " << pid << std::endl;
    }
}

void Process::continue_process()
{
    pid_t pid = this->process_id;

    int killing = kill(pid, SIGCONT);
    if (killing == 0)
    {
        std::cout << "Continued process " << pid << std::endl;
    }
}

void Process::kill_process()
{
    pid_t pid = this->process_id;

    int killing = kill(pid, SIGKILL);
    if (killing == 0)
    {
        std::cout << "Killed process " << pid << std::endl;
    }
}

std::vector<File> Process::list_open_files()
{
    int pid = this->process_id;
    std::string pathToFds = "/proc/" + std::to_string(pid) + "/fd";
    std::vector<File> returnVector;
    try
    {
        for (const auto &entry : std::filesystem::directory_iterator(pathToFds))
        {
            File file = File();
            file.set_fd(std::stoi(entry.path().filename()));
            std::filesystem::path target = std::filesystem::read_symlink(entry.path());
            std::string pathString{target.u8string()};
            if (pathString.find("socket") != std::string::npos)
            {
                file.set_type("local socket");
            }
            else
            {
                file.set_type("file");
                file.set_object(pathString);
            }
            returnVector.push_back(file);
        }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        //std::cout << "error accessing directory" << e.what() << std::endl;
    }
    return returnVector;
}

std::string getUser(unsigned long uid)
{
    struct passwd *pws;
    pws = getpwuid(uid);
    if (pws != NULL)
    {
        return pws->pw_name;
    }
    return "";
}

void Process::assign_user_name()
{
    int pid = this->process_id;
    unsigned long uid = 0;
    std::string pathToUID = "/proc/" + std::to_string(pid) + "/loginuid";
    std::ifstream uidFile(pathToUID);

    if (uidFile.is_open() && uidFile.peek() != EOF)
    {
        std::string readLine;
        std::getline(uidFile, readLine);
        uid = std::stoul(readLine);
    }
    if (uid != 0)
    {
        std::string username = getUser(uid);
        this->set_user(username);
        this->set_user_id(uid);
    }
    uidFile.close();
}

void Process::update_memory_load()
{
    int pid = this->process_id;
    std::string pathToMemory = "/proc/" + std::to_string(pid) + "/statm";
    std::ifstream memoryFile(pathToMemory);

    long page_size = sysconf(_SC_PAGESIZE) / 1024;

    if (memoryFile.is_open() && memoryFile.peek() != EOF)
    {
        std::string readLine;
        std::getline(memoryFile, readLine);
        std::vector<std::string> memoryVector = split_string(readLine);
        this->set_virtual_memory((std::stoul(memoryVector[0]) * page_size));
        this->set_memory_load((std::stoul(memoryVector[0]) * page_size));
        this->set_resident_memory((std::stoul(memoryVector[1]) * page_size));
        this->set_shared_memory((std::stoul(memoryVector[2]) * page_size));
    }
    memoryFile.close();
}

void Process::update_name_status()
{
    int pid = this->process_id;
    std::string pathToStats = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream statsFile(pathToStats);

    if (statsFile.is_open() && statsFile.peek() != EOF)
    {
        for (int lineNumber = 0; lineNumber < 7; lineNumber++)
        {
            std::string readLine;
            std::getline(statsFile, readLine);
            std::vector<std::string> statusVector = split_string(readLine);
            if (lineNumber == 0)
            {
                this->set_name(statusVector[1]);
            }
            else if (lineNumber == 2)
            {
                this->set_status(statusVector[1]);
            }
            else if (lineNumber == 6)
            {
                this->set_parent_pid(std::stoul(statusVector[1]));
            }
        }
    }
    statsFile.close();
}

void Process::update_time_parameters()
{
    unsigned long pid = this->process_id;

    std::ifstream statFile("/proc/" + std::to_string(pid) + "/stat");

    if (statFile.is_open() && statFile.peek() != EOF)
    {
        for (int i = 0; i < 22; i++)
        {
            std::string empty;
            statFile >> empty;
        }
        unsigned long long time_ticks; // time ticks since process started
        statFile >> time_ticks;
        statFile.close();

        // https://stackoverflow.com/questions/997512/string-representation-of-time-t
        std::time_t seconds_since_start = time_ticks / sysconf(_SC_CLK_TCK);
        std::time_t now = std::time(NULL);
        std::time_t started = now - seconds_since_start;
        // std::tm *ptm = std::localtime(&now);
        char cputime[32];
        char date_time_started[32];
        std::tm *cpu_time_date_tm = std::localtime(&seconds_since_start);
        std::tm *start_time_date_tm = std::localtime(&started);
        std::strftime(cputime, 32, "%H:%M:%S", cpu_time_date_tm);
        std::strftime(date_time_started, 32, "%d.%m.%Y %H:%M:%S", start_time_date_tm);
        std::string cpu_time_final(cpu_time);
        std::string date_time_final(date_time_started);
        this->set_cpu_time(cputime);
        this->set_time_started(date_time_started);
    }
}

void Process::update_self()
{
    this->assign_user_name();
    this->update_time_parameters();
    this->update_memory_load();
    this->update_name_status();
}
