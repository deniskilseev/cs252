#include "DetailedView.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>

#include <optional>
#include <vector>

#include "../BACKEND/Process.h"

void detailedView(Process process) {
    Fl_Window* actionWindow = new Fl_Window(400, 300, "Detailed View");

    Fl_Box* processOutput1 = new Fl_Box(10, 30, 380, 280);
    std::string view = "Process Name: " + process.get_name() + "\n"
        + "User: " + std::to_string(process.get_user_id()) + "\n"
        + "State: " + process.get_status() + "\n"
        + "Memory: " + std::to_string(process.get_virtual_memory()) + "\n"
        + "Virtual Memory: " + std::to_string(process.get_virtual_memory()) + "\n"
        + "Resident Memory: " + std::to_string(process.get_resident_memory()) + "\n"
        + "Shared Memory: " + std::to_string(process.get_shared_memory()) + "\n"
        + "CPU Time: " + process.get_cpu_time()  + "\n"
        + "Date/Time: " + process.get_time_started()  + "\n";

    processOutput1->label(view.c_str());
    processOutput1->redraw();

    actionWindow->end();
    actionWindow->show();
    Fl::run();
}
