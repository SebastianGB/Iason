# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/brunner/Iason/TTS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/brunner/Iason/TTS/qt-build

# Include any dependencies generated for this target.
include CMakeFiles/TTS.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TTS.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TTS.dir/flags.make

CMakeFiles/TTS.dir/src/espeak.cpp.o: CMakeFiles/TTS.dir/flags.make
CMakeFiles/TTS.dir/src/espeak.cpp.o: ../src/espeak.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/brunner/Iason/TTS/qt-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/TTS.dir/src/espeak.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/TTS.dir/src/espeak.cpp.o -c /home/brunner/Iason/TTS/src/espeak.cpp

CMakeFiles/TTS.dir/src/espeak.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TTS.dir/src/espeak.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/brunner/Iason/TTS/src/espeak.cpp > CMakeFiles/TTS.dir/src/espeak.cpp.i

CMakeFiles/TTS.dir/src/espeak.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TTS.dir/src/espeak.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/brunner/Iason/TTS/src/espeak.cpp -o CMakeFiles/TTS.dir/src/espeak.cpp.s

CMakeFiles/TTS.dir/src/espeak.cpp.o.requires:
.PHONY : CMakeFiles/TTS.dir/src/espeak.cpp.o.requires

CMakeFiles/TTS.dir/src/espeak.cpp.o.provides: CMakeFiles/TTS.dir/src/espeak.cpp.o.requires
	$(MAKE) -f CMakeFiles/TTS.dir/build.make CMakeFiles/TTS.dir/src/espeak.cpp.o.provides.build
.PHONY : CMakeFiles/TTS.dir/src/espeak.cpp.o.provides

CMakeFiles/TTS.dir/src/espeak.cpp.o.provides.build: CMakeFiles/TTS.dir/src/espeak.cpp.o

# Object files for target TTS
TTS_OBJECTS = \
"CMakeFiles/TTS.dir/src/espeak.cpp.o"

# External object files for target TTS
TTS_EXTERNAL_OBJECTS =

../bin/TTS: CMakeFiles/TTS.dir/src/espeak.cpp.o
../bin/TTS: CMakeFiles/TTS.dir/build.make
../bin/TTS: /usr/lib/x86_64-linux-gnu/libportaudio.so
../bin/TTS: /usr/lib/libespeak.so
../bin/TTS: CMakeFiles/TTS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../bin/TTS"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TTS.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TTS.dir/build: ../bin/TTS
.PHONY : CMakeFiles/TTS.dir/build

CMakeFiles/TTS.dir/requires: CMakeFiles/TTS.dir/src/espeak.cpp.o.requires
.PHONY : CMakeFiles/TTS.dir/requires

CMakeFiles/TTS.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TTS.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TTS.dir/clean

CMakeFiles/TTS.dir/depend:
	cd /home/brunner/Iason/TTS/qt-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/brunner/Iason/TTS /home/brunner/Iason/TTS /home/brunner/Iason/TTS/qt-build /home/brunner/Iason/TTS/qt-build /home/brunner/Iason/TTS/qt-build/CMakeFiles/TTS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TTS.dir/depend
