CXX = g++
CXXFLAGS = -g -I $(shell fltk-config --includedir)
LDFLAGS = -L $(shell fltk-config --libs)
LDLIBS = -lfltk -lfltk_images -lfltk_forms

BACKEND_DIR = ./src/BACKEND/
BACKEND_NAMES = CPUCore CPU CPUHandler File Memory MemoryHandler MemoryMap MountPoint Process ProcessHandler StorageHandler Network NetworkHandler
GUI_DIR = ./src/GUI/

BACKEND_H = $(addprefix $(BACKEND_DIR), $(addsuffix .h, $(BACKEND_NAMES)))
BACKEND_O = $(BACKEND_NAMES:=.o)
GUI_O = BasicSysInfo.o AllProcessesGUI.o TreeProcessesGUI.o UserProcessesGUI.o ProcessGUI.o ProcessActions.o OpenFiles.o MemoryMaps.o DetailedView.o CPUHistory.o CPUMemory.o NetworkGUI.o FileSystem.o
FILES_GUI_H = AllProcessesGUI.h TreeProcessesGUI.h UserProcessesGUI.h ProcessActions.h OpenFiles.h MemoryMaps.h DetailedView.h CPUHistory.h CPUMemory.h NetworkGUI.h FileSystem.h
GUI_H = $(addprefix $(GUI_DIR), $(FILES_GUI_H))
SRC_O = $(BACKEND_O) $(GUI_O)

OBJECT_DIR = build
OBJECTS = $(addprefix $(addsuffix /, $(OBJECT_DIR)), $(SRC_O))
TEST_OBJECTS = $(addprefix $(addsuffix /, $(OBJECT_DIR)), $(BACKEND_O) main.o)

TARGET = yasm
TEST = test.out

all: $(TARGET)

$(TEST): $(TEST_OBJECTS)
	$(CXX) $(LDFLAGS) -g -o $@ $^ $(LDLIBS)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(OBJECT_DIR):
	mkdir $(OBJECT_DIR)

$(OBJECT_DIR)/CPUCore.o: $(BACKEND_DIR)CPUCore.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(BACKEND_DIR)CPUCore.cpp

$(OBJECT_DIR)/CPU.o: $(BACKEND_DIR)CPU.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(BACKEND_DIR)CPU.cpp

$(OBJECT_DIR)/CPUHandler.o: $(BACKEND_DIR)CPUHandler.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(BACKEND_DIR)CPUHandler.cpp

$(OBJECT_DIR)/File.o: $(BACKEND_DIR)File.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(BACKEND_DIR)File.cpp

$(OBJECT_DIR)/Memory.o: $(BACKEND_DIR)Memory.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(BACKEND_DIR)Memory.cpp

$(OBJECT_DIR)/MemoryHandler.o: $(BACKEND_DIR)MemoryHandler.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(BACKEND_DIR)MemoryHandler.cpp

$(OBJECT_DIR)/MemoryMap.o: $(BACKEND_DIR)/MemoryMap.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(BACKEND_DIR)MemoryMap.cpp

$(OBJECT_DIR)/MountPoint.o: $(BACKEND_DIR)MountPoint.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(BACKEND_DIR)MountPoint.cpp

$(OBJECT_DIR)/Process.o: $(BACKEND_DIR)Process.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(BACKEND_DIR)Process.cpp

$(OBJECT_DIR)/ProcessHandler.o: $(BACKEND_DIR)ProcessHandler.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(BACKEND_DIR)ProcessHandler.cpp

$(OBJECT_DIR)/StorageHandler.o: $(BACKEND_DIR)StorageHandler.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(BACKEND_DIR)StorageHandler.cpp

$(OBJECT_DIR)/Network.o: $(BACKEND_DIR)Network.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(BACKEND_DIR)Network.cpp

$(OBJECT_DIR)/NetworkHandler.o: $(BACKEND_DIR)NetworkHandler.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(BACKEND_DIR)NetworkHandler.cpp

$(OBJECT_DIR)/main.o: ./src/main.cpp $(BACKEND_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $ ./src/main.cpp

$(OBJECT_DIR)/BasicSysInfo.o: $(GUI_DIR)BasicSysInfo.cpp $(BACKEND_H) $(GUI_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(GUI_DIR)BasicSysInfo.cpp

$(OBJECT_DIR)/AllProcessesGUI.o: $(GUI_DIR)AllProcessesGUI.cpp $(BACKEND_H) $(GUI_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(GUI_DIR)AllProcessesGUI.cpp

$(OBJECT_DIR)/TreeProcessesGUI.o: $(GUI_DIR)TreeProcessesGUI.cpp $(BACKEND_H) $(GUI_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(GUI_DIR)TreeProcessesGUI.cpp

$(OBJECT_DIR)/UserProcessesGUI.o: $(GUI_DIR)UserProcessesGUI.cpp $(BACKEND_H) $(GUI_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(GUI_DIR)UserProcessesGUI.cpp

$(OBJECT_DIR)/ProcessActions.o: $(GUI_DIR)ProcessActions.cpp $(BACKEND_H) $(GUI_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(GUI_DIR)ProcessActions.cpp

$(OBJECT_DIR)/OpenFiles.o: $(GUI_DIR)OpenFiles.cpp $(BACKEND_H) $(GUI_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(GUI_DIR)OpenFiles.cpp

$(OBJECT_DIR)/MemoryMaps.o: $(GUI_DIR)MemoryMaps.cpp $(BACKEND_H) $(GUI_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(GUI_DIR)MemoryMaps.cpp

$(OBJECT_DIR)/DetailedView.o: $(GUI_DIR)DetailedView.cpp $(BACKEND_H) $(GUI_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(GUI_DIR)DetailedView.cpp

$(OBJECT_DIR)/CPUHistory.o: $(GUI_DIR)CPUHistory.cpp $(BACKEND_H) $(GUI_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(GUI_DIR)CPUHistory.cpp

$(OBJECT_DIR)/CPUMemory.o: $(GUI_DIR)CPUMemory.cpp $(BACKEND_H) $(GUI_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(GUI_DIR)CPUMemory.cpp

$(OBJECT_DIR)/NetworkGUI.o: $(GUI_DIR)NetworkGUI.cpp $(BACKEND_H) $(GUI_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(GUI_DIR)NetworkGUI.cpp

$(OBJECT_DIR)/FileSystem.o: $(GUI_DIR)FileSystem.cpp $(BACKEND_H) $(GUI_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(GUI_DIR)FileSystem.cpp

$(OBJECT_DIR)/ProcessGUI.o: $(GUI_DIR)ProcessGUI.cpp $(BACKEND_H) $(GUI_H) $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(GUI_DIR)ProcessGUI.cpp

clean:
	rm -rf $(OBJECT_DIR) $(TARGET) $(TEST)

test: $(TEST)
