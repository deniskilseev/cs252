#include "MemoryMaps.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Multiline_Output.H>

#include <optional>
#include <vector>

#include "../BACKEND/MemoryMap.h"

void showMemoryMaps(std::vector<MemoryMap> memory_maps) {
    Fl_Window* actionWindow = new Fl_Window(400, 600, "Memory Maps");
    //Fl_Box* mapsOutput = new Fl_Box(10, 30, 380, 280);
    
    Fl_Scroll* scroll = new Fl_Scroll(10, 30, 380, 560);
    Fl_Multiline_Output* displayTextOutput = new Fl_Multiline_Output(10, 30, 20000, 20000);
    scroll->add(displayTextOutput);

    std::string mapsInfo = "";
    //std::cout << "memory maps len: " << memory_maps.size();
    for (int index = 0; index < memory_maps.size(); index ++) {
        MemoryMap map = memory_maps[index];
        mapsInfo += "Filename: " + map.get_filename() + "\n"
                                + " | VM Start: " + map.get_vm_start() + "\n"
                                + " | VM End: " + map.get_vm_end() + "\n"
                                + " | VM Size: " + std::to_string(map.get_vm_size()) + "\n"
                                + " | Flags: ";

        for (int flag : map.get_flags()) {
            mapsInfo += std::to_string(flag) + " ";
        }

	mapsInfo += "\n";

        mapsInfo += " | VM Offset: " + map.get_vm_offset() + "\n\n\n";
        //std::cout << mapsInfo;
    }
    displayTextOutput->value(mapsInfo.c_str());

    actionWindow->add(scroll);
    actionWindow->end();
    actionWindow->show();
}
