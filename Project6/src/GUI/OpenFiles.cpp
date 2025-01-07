#include "OpenFiles.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>

#include <optional>
#include <vector>

#include "../BACKEND/File.h"

void showOpenFiles(std::vector<File> files) {
    Fl_Window* actionWindow = new Fl_Window(400, 300, "Open Files");

    int y = 10;

    for (auto& file : files) {
        Fl_Output* fileOutput = new Fl_Output(10, y, 380, 20);

        std::string fileInfo = "File Descriptor: " 
                                + std::to_string(file.get_fd()) 
                                + " | Type: " + file.get_type();
        fileOutput->value(fileInfo.c_str());

        y += 30;
    }

    actionWindow->end();
    actionWindow->show();
}