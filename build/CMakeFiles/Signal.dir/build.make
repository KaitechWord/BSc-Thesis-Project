# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kajtekk/Documents/inzynierka/SignalNaive

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kajtekk/Documents/inzynierka/SignalNaive/build

# Include any dependencies generated for this target.
include CMakeFiles/Signal.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Signal.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Signal.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Signal.dir/flags.make

CMakeFiles/Signal.dir/source/main.cpp.o: CMakeFiles/Signal.dir/flags.make
CMakeFiles/Signal.dir/source/main.cpp.o: /home/kajtekk/Documents/inzynierka/SignalNaive/source/main.cpp
CMakeFiles/Signal.dir/source/main.cpp.o: CMakeFiles/Signal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kajtekk/Documents/inzynierka/SignalNaive/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Signal.dir/source/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Signal.dir/source/main.cpp.o -MF CMakeFiles/Signal.dir/source/main.cpp.o.d -o CMakeFiles/Signal.dir/source/main.cpp.o -c /home/kajtekk/Documents/inzynierka/SignalNaive/source/main.cpp

CMakeFiles/Signal.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Signal.dir/source/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kajtekk/Documents/inzynierka/SignalNaive/source/main.cpp > CMakeFiles/Signal.dir/source/main.cpp.i

CMakeFiles/Signal.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Signal.dir/source/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kajtekk/Documents/inzynierka/SignalNaive/source/main.cpp -o CMakeFiles/Signal.dir/source/main.cpp.s

CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.o: CMakeFiles/Signal.dir/flags.make
CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.o: /home/kajtekk/Documents/inzynierka/SignalNaive/source/FileManager/FileManager.cpp
CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.o: CMakeFiles/Signal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kajtekk/Documents/inzynierka/SignalNaive/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.o -MF CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.o.d -o CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.o -c /home/kajtekk/Documents/inzynierka/SignalNaive/source/FileManager/FileManager.cpp

CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kajtekk/Documents/inzynierka/SignalNaive/source/FileManager/FileManager.cpp > CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.i

CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kajtekk/Documents/inzynierka/SignalNaive/source/FileManager/FileManager.cpp -o CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.s

CMakeFiles/Signal.dir/source/Config/Config.cpp.o: CMakeFiles/Signal.dir/flags.make
CMakeFiles/Signal.dir/source/Config/Config.cpp.o: /home/kajtekk/Documents/inzynierka/SignalNaive/source/Config/Config.cpp
CMakeFiles/Signal.dir/source/Config/Config.cpp.o: CMakeFiles/Signal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kajtekk/Documents/inzynierka/SignalNaive/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Signal.dir/source/Config/Config.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Signal.dir/source/Config/Config.cpp.o -MF CMakeFiles/Signal.dir/source/Config/Config.cpp.o.d -o CMakeFiles/Signal.dir/source/Config/Config.cpp.o -c /home/kajtekk/Documents/inzynierka/SignalNaive/source/Config/Config.cpp

CMakeFiles/Signal.dir/source/Config/Config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Signal.dir/source/Config/Config.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kajtekk/Documents/inzynierka/SignalNaive/source/Config/Config.cpp > CMakeFiles/Signal.dir/source/Config/Config.cpp.i

CMakeFiles/Signal.dir/source/Config/Config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Signal.dir/source/Config/Config.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kajtekk/Documents/inzynierka/SignalNaive/source/Config/Config.cpp -o CMakeFiles/Signal.dir/source/Config/Config.cpp.s

CMakeFiles/Signal.dir/source/Filter/Filter.cpp.o: CMakeFiles/Signal.dir/flags.make
CMakeFiles/Signal.dir/source/Filter/Filter.cpp.o: /home/kajtekk/Documents/inzynierka/SignalNaive/source/Filter/Filter.cpp
CMakeFiles/Signal.dir/source/Filter/Filter.cpp.o: CMakeFiles/Signal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kajtekk/Documents/inzynierka/SignalNaive/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Signal.dir/source/Filter/Filter.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Signal.dir/source/Filter/Filter.cpp.o -MF CMakeFiles/Signal.dir/source/Filter/Filter.cpp.o.d -o CMakeFiles/Signal.dir/source/Filter/Filter.cpp.o -c /home/kajtekk/Documents/inzynierka/SignalNaive/source/Filter/Filter.cpp

CMakeFiles/Signal.dir/source/Filter/Filter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Signal.dir/source/Filter/Filter.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kajtekk/Documents/inzynierka/SignalNaive/source/Filter/Filter.cpp > CMakeFiles/Signal.dir/source/Filter/Filter.cpp.i

CMakeFiles/Signal.dir/source/Filter/Filter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Signal.dir/source/Filter/Filter.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kajtekk/Documents/inzynierka/SignalNaive/source/Filter/Filter.cpp -o CMakeFiles/Signal.dir/source/Filter/Filter.cpp.s

CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.o: CMakeFiles/Signal.dir/flags.make
CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.o: /home/kajtekk/Documents/inzynierka/SignalNaive/source/ImageFilter/ImageFilter.cpp
CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.o: CMakeFiles/Signal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kajtekk/Documents/inzynierka/SignalNaive/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.o -MF CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.o.d -o CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.o -c /home/kajtekk/Documents/inzynierka/SignalNaive/source/ImageFilter/ImageFilter.cpp

CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kajtekk/Documents/inzynierka/SignalNaive/source/ImageFilter/ImageFilter.cpp > CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.i

CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kajtekk/Documents/inzynierka/SignalNaive/source/ImageFilter/ImageFilter.cpp -o CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.s

# Object files for target Signal
Signal_OBJECTS = \
"CMakeFiles/Signal.dir/source/main.cpp.o" \
"CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.o" \
"CMakeFiles/Signal.dir/source/Config/Config.cpp.o" \
"CMakeFiles/Signal.dir/source/Filter/Filter.cpp.o" \
"CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.o"

# External object files for target Signal
Signal_EXTERNAL_OBJECTS =

Signal: CMakeFiles/Signal.dir/source/main.cpp.o
Signal: CMakeFiles/Signal.dir/source/FileManager/FileManager.cpp.o
Signal: CMakeFiles/Signal.dir/source/Config/Config.cpp.o
Signal: CMakeFiles/Signal.dir/source/Filter/Filter.cpp.o
Signal: CMakeFiles/Signal.dir/source/ImageFilter/ImageFilter.cpp.o
Signal: CMakeFiles/Signal.dir/build.make
Signal: /usr/lib/libboost_program_options.so.1.83.0
Signal: CMakeFiles/Signal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/kajtekk/Documents/inzynierka/SignalNaive/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable Signal"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Signal.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Signal.dir/build: Signal
.PHONY : CMakeFiles/Signal.dir/build

CMakeFiles/Signal.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Signal.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Signal.dir/clean

CMakeFiles/Signal.dir/depend:
	cd /home/kajtekk/Documents/inzynierka/SignalNaive/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kajtekk/Documents/inzynierka/SignalNaive /home/kajtekk/Documents/inzynierka/SignalNaive /home/kajtekk/Documents/inzynierka/SignalNaive/build /home/kajtekk/Documents/inzynierka/SignalNaive/build /home/kajtekk/Documents/inzynierka/SignalNaive/build/CMakeFiles/Signal.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Signal.dir/depend
