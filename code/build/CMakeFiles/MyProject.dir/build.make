# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/mattiacampana/Developer/PII_Campana/code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mattiacampana/Developer/PII_Campana/code/build

# Include any dependencies generated for this target.
include CMakeFiles/MyProject.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MyProject.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MyProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyProject.dir/flags.make

CMakeFiles/MyProject.dir/src/arrays.c.o: CMakeFiles/MyProject.dir/flags.make
CMakeFiles/MyProject.dir/src/arrays.c.o: /home/mattiacampana/Developer/PII_Campana/code/src/arrays.c
CMakeFiles/MyProject.dir/src/arrays.c.o: CMakeFiles/MyProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/mattiacampana/Developer/PII_Campana/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/MyProject.dir/src/arrays.c.o"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/MyProject.dir/src/arrays.c.o -MF CMakeFiles/MyProject.dir/src/arrays.c.o.d -o CMakeFiles/MyProject.dir/src/arrays.c.o -c /home/mattiacampana/Developer/PII_Campana/code/src/arrays.c

CMakeFiles/MyProject.dir/src/arrays.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/MyProject.dir/src/arrays.c.i"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mattiacampana/Developer/PII_Campana/code/src/arrays.c > CMakeFiles/MyProject.dir/src/arrays.c.i

CMakeFiles/MyProject.dir/src/arrays.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/MyProject.dir/src/arrays.c.s"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mattiacampana/Developer/PII_Campana/code/src/arrays.c -o CMakeFiles/MyProject.dir/src/arrays.c.s

CMakeFiles/MyProject.dir/src/bitop.c.o: CMakeFiles/MyProject.dir/flags.make
CMakeFiles/MyProject.dir/src/bitop.c.o: /home/mattiacampana/Developer/PII_Campana/code/src/bitop.c
CMakeFiles/MyProject.dir/src/bitop.c.o: CMakeFiles/MyProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/mattiacampana/Developer/PII_Campana/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/MyProject.dir/src/bitop.c.o"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/MyProject.dir/src/bitop.c.o -MF CMakeFiles/MyProject.dir/src/bitop.c.o.d -o CMakeFiles/MyProject.dir/src/bitop.c.o -c /home/mattiacampana/Developer/PII_Campana/code/src/bitop.c

CMakeFiles/MyProject.dir/src/bitop.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/MyProject.dir/src/bitop.c.i"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mattiacampana/Developer/PII_Campana/code/src/bitop.c > CMakeFiles/MyProject.dir/src/bitop.c.i

CMakeFiles/MyProject.dir/src/bitop.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/MyProject.dir/src/bitop.c.s"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mattiacampana/Developer/PII_Campana/code/src/bitop.c -o CMakeFiles/MyProject.dir/src/bitop.c.s

CMakeFiles/MyProject.dir/src/main.c.o: CMakeFiles/MyProject.dir/flags.make
CMakeFiles/MyProject.dir/src/main.c.o: /home/mattiacampana/Developer/PII_Campana/code/src/main.c
CMakeFiles/MyProject.dir/src/main.c.o: CMakeFiles/MyProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/mattiacampana/Developer/PII_Campana/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/MyProject.dir/src/main.c.o"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/MyProject.dir/src/main.c.o -MF CMakeFiles/MyProject.dir/src/main.c.o.d -o CMakeFiles/MyProject.dir/src/main.c.o -c /home/mattiacampana/Developer/PII_Campana/code/src/main.c

CMakeFiles/MyProject.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/MyProject.dir/src/main.c.i"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mattiacampana/Developer/PII_Campana/code/src/main.c > CMakeFiles/MyProject.dir/src/main.c.i

CMakeFiles/MyProject.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/MyProject.dir/src/main.c.s"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mattiacampana/Developer/PII_Campana/code/src/main.c -o CMakeFiles/MyProject.dir/src/main.c.s

CMakeFiles/MyProject.dir/src/seed.c.o: CMakeFiles/MyProject.dir/flags.make
CMakeFiles/MyProject.dir/src/seed.c.o: /home/mattiacampana/Developer/PII_Campana/code/src/seed.c
CMakeFiles/MyProject.dir/src/seed.c.o: CMakeFiles/MyProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/mattiacampana/Developer/PII_Campana/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/MyProject.dir/src/seed.c.o"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/MyProject.dir/src/seed.c.o -MF CMakeFiles/MyProject.dir/src/seed.c.o.d -o CMakeFiles/MyProject.dir/src/seed.c.o -c /home/mattiacampana/Developer/PII_Campana/code/src/seed.c

CMakeFiles/MyProject.dir/src/seed.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/MyProject.dir/src/seed.c.i"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mattiacampana/Developer/PII_Campana/code/src/seed.c > CMakeFiles/MyProject.dir/src/seed.c.i

CMakeFiles/MyProject.dir/src/seed.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/MyProject.dir/src/seed.c.s"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mattiacampana/Developer/PII_Campana/code/src/seed.c -o CMakeFiles/MyProject.dir/src/seed.c.s

CMakeFiles/MyProject.dir/src/xoshiro.c.o: CMakeFiles/MyProject.dir/flags.make
CMakeFiles/MyProject.dir/src/xoshiro.c.o: /home/mattiacampana/Developer/PII_Campana/code/src/xoshiro.c
CMakeFiles/MyProject.dir/src/xoshiro.c.o: CMakeFiles/MyProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/mattiacampana/Developer/PII_Campana/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/MyProject.dir/src/xoshiro.c.o"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/MyProject.dir/src/xoshiro.c.o -MF CMakeFiles/MyProject.dir/src/xoshiro.c.o.d -o CMakeFiles/MyProject.dir/src/xoshiro.c.o -c /home/mattiacampana/Developer/PII_Campana/code/src/xoshiro.c

CMakeFiles/MyProject.dir/src/xoshiro.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/MyProject.dir/src/xoshiro.c.i"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mattiacampana/Developer/PII_Campana/code/src/xoshiro.c > CMakeFiles/MyProject.dir/src/xoshiro.c.i

CMakeFiles/MyProject.dir/src/xoshiro.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/MyProject.dir/src/xoshiro.c.s"
	/usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mattiacampana/Developer/PII_Campana/code/src/xoshiro.c -o CMakeFiles/MyProject.dir/src/xoshiro.c.s

# Object files for target MyProject
MyProject_OBJECTS = \
"CMakeFiles/MyProject.dir/src/arrays.c.o" \
"CMakeFiles/MyProject.dir/src/bitop.c.o" \
"CMakeFiles/MyProject.dir/src/main.c.o" \
"CMakeFiles/MyProject.dir/src/seed.c.o" \
"CMakeFiles/MyProject.dir/src/xoshiro.c.o"

# External object files for target MyProject
MyProject_EXTERNAL_OBJECTS =

bin/MyProject: CMakeFiles/MyProject.dir/src/arrays.c.o
bin/MyProject: CMakeFiles/MyProject.dir/src/bitop.c.o
bin/MyProject: CMakeFiles/MyProject.dir/src/main.c.o
bin/MyProject: CMakeFiles/MyProject.dir/src/seed.c.o
bin/MyProject: CMakeFiles/MyProject.dir/src/xoshiro.c.o
bin/MyProject: CMakeFiles/MyProject.dir/build.make
bin/MyProject: CMakeFiles/MyProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/mattiacampana/Developer/PII_Campana/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable bin/MyProject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MyProject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MyProject.dir/build: bin/MyProject
.PHONY : CMakeFiles/MyProject.dir/build

CMakeFiles/MyProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MyProject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MyProject.dir/clean

CMakeFiles/MyProject.dir/depend:
	cd /home/mattiacampana/Developer/PII_Campana/code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mattiacampana/Developer/PII_Campana/code /home/mattiacampana/Developer/PII_Campana/code /home/mattiacampana/Developer/PII_Campana/code/build /home/mattiacampana/Developer/PII_Campana/code/build /home/mattiacampana/Developer/PII_Campana/code/build/CMakeFiles/MyProject.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/MyProject.dir/depend

