// MemoryMap.h
#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include <iostream>
#include <string>
#include <vector>

class MemoryMap
{
private:
    std::string filename, vm_start, vm_end, vm_offset, io_device;
    unsigned long vm_size, private_clean, private_dirty, shared_clean, shared_dirty, inode;
    std::vector<int> flags;

public:
    MemoryMap()
    {
        this->flags = std::vector<int>(0); //
        this->filename = "";
        this->vm_start = "";
        this->vm_end = "";
        this->vm_size = 0;
        this->vm_offset = "";
        this->io_device = "";
        this->private_clean = 0;
        this->private_dirty = 0;
        this->shared_dirty = 0;
        this->shared_clean = 0;
        this->inode = 0;
    }

    MemoryMap(int argc, char **argv); // TODO create the memory map and determine what arguments to put in

    void print()
    {
        std::cout << "Flags: ";
        for (int i = 0; i < this->flags.size(); i++)
        {
            std::cout << this->flags.at(i) << " ";
        }
        std::cout << "\n";
        std::cout << "Filename: " << this->filename << "\n";
        std::cout << "VM_start: " << this->vm_start << "\n";
        std::cout << "VM_end: " << this->vm_end << "\n";
        std::cout << "VM_size: " << this->vm_size << "\n";
        std::cout << "VM_offset: " << this->vm_offset << "\n";
        std::cout << "IO device: " << this->io_device << "\n";
        std::cout << "Inode device: " << this->inode << "\n";
        std::cout << "priv cl: " << this->private_clean << "\n";
        std::cout << "priv dir: " << this->private_dirty << "\n";
        std::cout << "shared cl: " << this->shared_clean << "\n";
        std::cout << "shared dirty: " << this->shared_dirty << "\n";
    }

    std::string get_filename()
    {
        return this->filename;
    }

    void set_filename(std::string name)
    {
        this->filename = name;
    }

    std::string get_vm_start()
    {
        return this->vm_start;
    }

    void set_vm_start(std::string index)
    {
        this->vm_start = index;
    }

    std::string get_vm_end()
    {
        return this->vm_end;
    }

    void set_vm_end(std::string index)
    {
        this->vm_end = index;
    }

    unsigned long get_vm_size()
    {
        return this->vm_size;
    }

    void set_vm_size(unsigned long size)
    {
        this->vm_size = size;
    }

    std::vector<int> get_flags()
    {
        return this->flags;
    }

    void set_flags(std::vector<int> flags)
    {
        this->flags = flags;
    }

    std::string get_vm_offset()
    {
        return this->vm_offset;
    }

    void set_vm_offset(std::string offset)
    {
        this->vm_offset = offset;
    }

    std::string get_io_device()
    {
        return this->io_device;
    }

    void set_io_device(std::string device)
    {
        this->io_device = device;
    }

    unsigned long get_private_clean()
    {
        return this->private_clean;
    }

    void set_private_clean(unsigned long bit)
    {
        this->private_clean = bit;
    }

    unsigned long get_private_dirty()
    {
        return this->private_dirty;
    }

    void set_private_dirty(unsigned long bit)
    {
        this->private_dirty = bit;
    }

    unsigned long get_shared_clean()
    {
        return this->shared_clean;
    }

    void set_shared_clean(unsigned long bit)
    {
        this->shared_clean = bit;
    }

    unsigned long get_shared_dirty()
    {
        return this->shared_dirty;
    }

    void set_shared_dirty(unsigned long bit)
    {
        this->shared_dirty = bit;
    }

    unsigned long get_inode()
    {
        return this->inode;
    }

    void set_inode(unsigned long inode)
    {
        this->inode = inode;
    }
};

#endif
