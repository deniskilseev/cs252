// Process.h
#ifndef PROCESS_H
#define PROCESS_H

#include "File.h"
#include "MemoryMap.h"

#include <string>
#include <string.h>
#include <vector>

class Process
{
private:
	unsigned long process_id;
	unsigned long user_id;
	std::string name;
	std::string status;
	int cpu_load;	 // Percentage of CPU
	int memory_load; // KiB
	unsigned long parent_processID;
	std::vector<unsigned long> children_processIDs;
	std::string user; // make this and below
	unsigned long virtual_memory_load;
	unsigned long resident_memory_load;
	unsigned long shared_memory_load;
	std::string cpu_time;
	std::string date_time_started;

public:
	Process()
	{
		this->process_id = 0;
		this->name = "";
		this->status = "";
		this->cpu_load = -1;
		this->memory_load = -1;
		this->user_id = 0;
		this->parent_processID = 0;
	}

	Process(int pid, std::string name)
	{
		this->process_id = pid;
		this->name = name;
	}

	Process(int pid)
	{
		this->process_id = pid;
		this->update_self();
	}

	std::string get_user()
	{
		return this->user;
	}

	void set_name(std::string name)
	{
		this->name = name;
	}

	void set_user(std::string user)
	{
		this->user = user;
	}

	void set_shared_memory(unsigned long memory)
	{
		this->shared_memory_load = memory;
	}

	unsigned long get_shared_memory()
	{
		return this->shared_memory_load;
	}

	void set_virtual_memory(unsigned long memory)
	{
		this->virtual_memory_load = memory;
	}

	unsigned long get_virtual_memory()
	{
		return this->virtual_memory_load;
	}

	unsigned long get_user_id()
	{
		return this->user_id;
	}

	void set_user_id(unsigned long id)
	{
		this->user_id = id;
	}

	void set_resident_memory(unsigned long memory)
	{
		this->resident_memory_load = memory;
	}

	unsigned long get_resident_memory()
	{
		return this->resident_memory_load;
	}

	unsigned long get_pid()
	{
		return this->process_id;
	}

	unsigned long get_parent_pid()
	{
		return this->parent_processID;
	}

	void set_parent_pid(unsigned long pid)
	{
		this->parent_processID = pid;
	}

	std::vector<unsigned long> get_children_pids()
	{
		return this->children_processIDs;
	}

	std::string get_name()
	{
		return this->name;
	}

	std::string get_status()
	{
		if (this->status == "R")
		{
			return "Running";
		}
		if (this->status == "S")
		{
			return "Sleeping";
		}
		if (this->status == "I")
		{
			return "Idle";
		}
		return this->status;
	}

	void set_status(std::string status)
	{
		this->status = status;
	}

	int get_cpu_load()
	{
		return this->cpu_load;
	}

	void set_cpu_load(int cpu_load)
	{
		this->cpu_load = cpu_load;
	}

	int get_memory_load()
	{
		return this->memory_load;
	}

	void set_memory_load(int memory_load)
	{
		this->memory_load = memory_load;
	}

	void free_child_pids()
	{
		this->children_processIDs = {};
	}

	void push_child_process(unsigned long pid)
	{
		this->children_processIDs.push_back(pid);
	}

	std::string get_cpu_time()
	{
		return this->cpu_time;
	}

	void set_cpu_time(std::string cpu_time)
	{
		this->cpu_time = cpu_time;
	}

	std::string get_time_started()
	{
		return this->date_time_started;
	}

	void set_time_started(std::string date_time)
	{
		this->date_time_started = date_time;
	}

	void stop_process();					   // TODO
	void continue_process();				   // TODO
	void kill_process();					   // TODO
	std::vector<MemoryMap> list_memory_maps(); // TODO
	std::vector<File> list_open_files();	   // TODO
	void update_self();						   // TODO
	void assign_user_name();
	void update_memory_load();
	void update_time_parameters();
	void update_name_status();
};

#endif
