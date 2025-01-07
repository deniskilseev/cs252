#include "AllProcessesGUI.h"
#include "UserProcessesGUI.h"
#include "TreeProcessesGUI.h"
#include "BasicSysInfo.h"
#include "ProcessActions.h"
#include "FileSystem.h"
#include "CPUHistory.h"
#include "CPUMemory.h"
#include "NetworkGUI.h"

#include "../BACKEND/ProcessHandler.h"

#include <cstdint>
#include <iostream>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Chart.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Multiline_Output.H>

enum class ViewType {
    System,
    AllProcesses,
    UserProcesses,
    TreeView,
    Graphs,
    Files,
    None
};

ViewType currentView = ViewType::None;
Fl_Box* displayBox;
Fl_Chart *cpuChart;
Fl_Chart *memoryChart;
Fl_Chart *networkChart;
Fl_Box *cpuLabel;
Fl_Box *memoryLabel;
Fl_Box *networkLabel;
Fl_Scroll* scroll;
Fl_Multiline_Output* displayTextOutput;

void updateDisplay() {
    static std::string displayText;
    switch (currentView) {
        case ViewType::System:
            displayText = getSystemInfo();
            break;
        case ViewType::AllProcesses: {
            ProcessHandler handler = ProcessHandler();
            displayText = showAllProcessesGUI(handler);
            break;
        }
        case ViewType::UserProcesses: {
            ProcessHandler handler2 = ProcessHandler();
            displayText = showUserProcessesGUI(handler2);
            break;
        }
        case ViewType::TreeView: {
            ProcessHandler handler3 = ProcessHandler();
            displayText = showTreeProcessesGUI(handler3);
            break;
        }
        case ViewType::Graphs:
            break;
        case ViewType::Files:
            displayText = filesString();
            break;
        default:
            displayText = "Select a view";
    }
    if (currentView == ViewType::Graphs) {
        cpuChart->show();
        memoryChart->show();
        networkChart->show();
        cpuLabel->show();
        memoryLabel->show();
        networkLabel->show();
        displayBox->hide();
        scroll->hide();
        return;
    } else {
        cpuChart->hide();
        memoryChart->hide();
        networkChart->hide();
        cpuLabel->hide();
        memoryLabel->hide();
        networkLabel->hide();
        //displayBox->show();
        scroll->show();
    }

    displayTextOutput->value(displayText.c_str());
    displayTextOutput->redraw();
    //displayBox->label(displayText.c_str());
    //displayBox->redraw();
}

void showProcessActionsWrapper(Fl_Widget*, void*) {
    showProcessActionsWindow();
}

const int maxLines = 20;
const int maxPointsPerLine = 50;
std::vector<int> linePositions(maxLines, 0);

void updateGraphs(void*) {
    std::vector<double> cpuValues = cpuHistory();
    std::vector<unsigned long> memoryValues = cpuMemory();
    std::vector<unsigned long> networkValues = networkString();

    cpuChart->clear();
    memoryChart->clear();
    networkChart->clear();

    std::string double_num;
    int dec_pos;

    for (size_t i = 0; i < maxLines; i++) {
        double_num = std::to_string(cpuValues[i]);
        dec_pos = double_num.find('.');
        double_num = double_num.substr(0, dec_pos + 2);
        std::string cpuLabel = "CPU" + std::to_string(i + 1) + "\n" + double_num;
        //std::cout << cpuLabel << "\n";
        if (cpuValues[i] < 100 && cpuValues[i] >= 0) {
            cpuChart->add(cpuValues[i], cpuLabel.c_str(), FL_RED + i);
        }
    }

    double_num = std::to_string(static_cast<double>(memoryValues[0]));
    dec_pos = double_num.find('.');
    double_num = double_num.substr(0, dec_pos);
    memoryChart->add(static_cast<double>(memoryValues[0]), ("RAM: " + double_num).c_str(), FL_GREEN);
    
    double_num = std::to_string(static_cast<double>(memoryValues[1]));
    dec_pos = double_num.find('.');
    double_num = double_num.substr(0, dec_pos);
    memoryChart->add(static_cast<double>(memoryValues[1]), ("swap: " + double_num).c_str(), FL_GREEN + 1);

    double_num = std::to_string(static_cast<double>(networkValues[0]));
    dec_pos = double_num.find('.');
    double_num = double_num.substr(0, dec_pos);
    networkChart->add(static_cast<double>(networkValues[0]), ("Upload Speed: " + double_num).c_str(), FL_BLUE);
    
    double_num = std::to_string(static_cast<double>(networkValues[1]));
    dec_pos = double_num.find('.');
    double_num = double_num.substr(0, dec_pos);
    networkChart->add(static_cast<double>(networkValues[1]), ("Download Speed: " + double_num).c_str(), FL_BLUE + 1);

    cpuChart->redraw();
    memoryChart->redraw();
    networkChart->redraw();

    Fl::repeat_timeout(1.0, updateGraphs);
}

void menu_callback(Fl_Widget*, void* data) {
    int choice = (int)(intptr_t)data;
    switch (choice) {
        case 0:
            currentView = ViewType::System;
            break;
        case 1:
            currentView = ViewType::AllProcesses;
            break;
        case 2:
            currentView = ViewType::UserProcesses;
            break;
        case 3:
            currentView = ViewType::TreeView;
            break;
        case 4:
            break;
        case 5:
            currentView = ViewType::Graphs;
            Fl::add_timeout(1.0, updateGraphs);
            break;
        case 6:
            currentView = ViewType::Files;
            break;
    }
    updateDisplay();
}

int main() {
    Fl_Window* window = new Fl_Window(1000, 600, "Process Viewer");
    Fl_Menu_Bar* menu = new Fl_Menu_Bar(0, 0, 1000, 25);

    menu->add("System", 0, menu_callback, (void*)0);
    menu->add("View Processes/All Processes", 0, menu_callback, (void*)1);
    menu->add("View Processes/User Processes", 0, menu_callback, (void*)2);
    menu->add("View Processes/Tree View", 0, menu_callback, (void*)3);
    menu->add("View Processes/Refresh", 0, menu_callback, (void*)4);
    menu->add("Process Actions", 0, showProcessActionsWrapper);
    menu->add("Graphs", 0, menu_callback, (void*)5);
    menu->add("File Systems", 0, menu_callback, (void*)6);

    displayBox = new Fl_Box(10, 30, 950, 560, "Select a view");

    scroll = new Fl_Scroll(10, 30, 950, 560);
    displayTextOutput = new Fl_Multiline_Output(10, 30, 10000, 10000);
    scroll->end();
    scroll->add(displayTextOutput);

    cpuChart = new Fl_Chart(50, 55, 820, 150, "CPU Usage");
    cpuChart->type(FL_SPIKE_CHART);
    cpuLabel = new Fl_Box(5, 130, 35, 150, "CPU%");
    cpuLabel->labelsize(12);
    cpuLabel->align(FL_ALIGN_INSIDE | FL_ALIGN_TOP);

    memoryChart = new Fl_Chart(50, 230, 780, 150, "Memory Usage");
    memoryChart->type(FL_SPIKE_CHART);
    memoryLabel = new Fl_Box(10, 280, 20, 150, "Load\n(KiB)");
    memoryLabel->labelsize(12);
    memoryLabel->align(FL_ALIGN_INSIDE | FL_ALIGN_TOP);

    networkChart = new Fl_Chart(50, 400, 780, 150, "Network Activity");
    networkChart->type(FL_SPIKE_CHART);
    networkLabel = new Fl_Box(10, 450, 20, 150, "(KiB/s)");
    networkLabel->labelsize(12);
    networkLabel->align(FL_ALIGN_INSIDE | FL_ALIGN_TOP);

    cpuChart->hide();
    memoryChart->hide();
    networkChart->hide();
    cpuLabel->hide();
    memoryLabel->hide();
    networkLabel->hide();
    scroll->hide();

    window->end();
    window->show();
    return Fl::run();
}
