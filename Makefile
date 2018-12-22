# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jyshi/ice

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jyshi/ice

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/jyshi/ice/CMakeFiles /home/jyshi/ice/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/jyshi/ice/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named ice

# Build rule for target.
ice: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 ice
.PHONY : ice

# fast build rule for target.
ice/fast:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/build
.PHONY : ice/fast

src/main.o: src/main.cpp.o

.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/network.o: src/network.cpp.o

.PHONY : src/network.o

# target to build an object file
src/network.cpp.o:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/network.cpp.o
.PHONY : src/network.cpp.o

src/network.i: src/network.cpp.i

.PHONY : src/network.i

# target to preprocess a source file
src/network.cpp.i:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/network.cpp.i
.PHONY : src/network.cpp.i

src/network.s: src/network.cpp.s

.PHONY : src/network.s

# target to generate assembly for a file
src/network.cpp.s:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/network.cpp.s
.PHONY : src/network.cpp.s

src/server.o: src/server.cpp.o

.PHONY : src/server.o

# target to build an object file
src/server.cpp.o:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/server.cpp.o
.PHONY : src/server.cpp.o

src/server.i: src/server.cpp.i

.PHONY : src/server.i

# target to preprocess a source file
src/server.cpp.i:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/server.cpp.i
.PHONY : src/server.cpp.i

src/server.s: src/server.cpp.s

.PHONY : src/server.s

# target to generate assembly for a file
src/server.cpp.s:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/server.cpp.s
.PHONY : src/server.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... ice"
	@echo "... edit_cache"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/network.o"
	@echo "... src/network.i"
	@echo "... src/network.s"
	@echo "... src/server.o"
	@echo "... src/server.i"
	@echo "... src/server.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

