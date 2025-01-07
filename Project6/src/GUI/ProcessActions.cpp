#include "ProcessActions.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <optional>

#include "../BACKEND/ProcessHandler.h"
#include "../BACKEND/Process.h"

#include "OpenFiles.h"
#include "MemoryMaps.h"
#include "DetailedView.h"

void stopButtonCallback(Fl_Widget*, void*);
void continueButtonCallback(Fl_Widget*, void*);
void killButtonCallback(Fl_Widget*, void*);
void listMemMapsButtonCallback(Fl_Widget*, void*);
void listOpenFilesButtonCallback(Fl_Widget*, void*);
void detailedViewCallBack(Fl_Widget*, void*);

void showProcessActionsWindow() {
    Fl_Window* actionWindow = new Fl_Window(400, 300, "Process Actions");
    Fl_Input* pidInput = new Fl_Input(120, 20, 260, 30, "Process ID:");

    Fl_Button* stopButton = new Fl_Button(20, 70, 80, 30, "Stop");
    stopButton->callback(stopButtonCallback, pidInput);

    Fl_Button* continueButton = new Fl_Button(110, 70, 80, 30, "Continue");
    continueButton->callback(continueButtonCallback, pidInput);

    Fl_Button* killButton = new Fl_Button(200, 70, 80, 30, "Kill");
    killButton->callback(killButtonCallback, pidInput);

    Fl_Button* listMemMapsButton = new Fl_Button(20, 110, 120, 30, "List Memory Maps");
    listMemMapsButton->callback(listMemMapsButtonCallback, pidInput);

    Fl_Button* listOpenFilesButton = new Fl_Button(150, 110, 120, 30, "List Open Files");
    listOpenFilesButton->callback(listOpenFilesButtonCallback, pidInput);

    Fl_Button* detailedView = new Fl_Button(150, 110, 120, 30, "Detailed View");
    detailedView->callback(detailedViewCallBack, pidInput);

    actionWindow->end();
    actionWindow->show();
}

std::optional<Process> getProcess(int pid) {
    ProcessHandler handler = ProcessHandler();
    std::vector<Process> processes = handler.get_processes();
    for (Process& process : processes) {
        if (process.get_pid() == pid) {
            return process;
        }
    }
    return std::nullopt;
}

bool isInteger(std::string str) {
    try {
        std::stoi(str);
        return true;
    } catch (std::invalid_argument&) {
        return false;
    }
}

void stopButtonCallback(Fl_Widget*, void* data) {
    Fl_Input* pidInput = (Fl_Input*)data;
    if (!isInteger(pidInput->value())) return;
    int pid = std::stoi(pidInput->value());
    auto process = getProcess(pid);
    if (process) {
        process->stop_process();
    }
}

void continueButtonCallback(Fl_Widget*, void* data) {
    Fl_Input* pidInput = (Fl_Input*)data;
    if (!isInteger(pidInput->value())) return;
    int pid = std::stoi(pidInput->value());
    auto process = getProcess(pid);
    if (process) {
        process->continue_process();
    }
}

void killButtonCallback(Fl_Widget*, void* data) {
    Fl_Input* pidInput = (Fl_Input*)data;
    if (!isInteger(pidInput->value())) return;
    int pid = std::stoi(pidInput->value());
    auto process = getProcess(pid);
    if (process) {
        process->kill_process();
    }
}

void listMemMapsButtonCallback(Fl_Widget*, void* data) {
    Fl_Input* pidInput = (Fl_Input*)data;
    if (!isInteger(pidInput->value())) return;
    int pid = std::stoi(pidInput->value());
    auto process = getProcess(pid);
    if (process) {
        std::vector<MemoryMap> memory_maps = process->list_memory_maps();
        showMemoryMaps(memory_maps);
    }
}

void listOpenFilesButtonCallback(Fl_Widget*, void* data) {
    Fl_Input* pidInput = (Fl_Input*)data;
    if (!isInteger(pidInput->value())) return;
    int pid = std::stoi(pidInput->value());
    auto process = getProcess(pid);
    if (process) {
        std::vector<File> files = process->list_open_files();
        showOpenFiles(files);
    }
}

void detailedViewCallBack(Fl_Widget*, void* data) {
    Fl_Input* pidInput = (Fl_Input*)data;
    if (!isInteger(pidInput->value())) return;
    int pid = std::stoi(pidInput->value());
    auto process = getProcess(pid);
    if (process) {
        detailedView(*process);
    }
}
