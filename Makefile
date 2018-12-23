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

libs/http_parser.o: libs/http_parser.cc.o

.PHONY : libs/http_parser.o

# target to build an object file
libs/http_parser.cc.o:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/libs/http_parser.cc.o
.PHONY : libs/http_parser.cc.o

libs/http_parser.i: libs/http_parser.cc.i

.PHONY : libs/http_parser.i

# target to preprocess a source file
libs/http_parser.cc.i:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/libs/http_parser.cc.i
.PHONY : libs/http_parser.cc.i

libs/http_parser.s: libs/http_parser.cc.s

.PHONY : libs/http_parser.s

# target to generate assembly for a file
libs/http_parser.cc.s:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/libs/http_parser.cc.s
.PHONY : libs/http_parser.cc.s

src/http.o: src/http.cc.o

.PHONY : src/http.o

# target to build an object file
src/http.cc.o:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/http.cc.o
.PHONY : src/http.cc.o

src/http.i: src/http.cc.i

.PHONY : src/http.i

# target to preprocess a source file
src/http.cc.i:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/http.cc.i
.PHONY : src/http.cc.i

src/http.s: src/http.cc.s

.PHONY : src/http.s

# target to generate assembly for a file
src/http.cc.s:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/http.cc.s
.PHONY : src/http.cc.s

src/main.o: src/main.cc.o

.PHONY : src/main.o

# target to build an object file
src/main.cc.o:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/main.cc.o
.PHONY : src/main.cc.o

src/main.i: src/main.cc.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cc.i:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/main.cc.i
.PHONY : src/main.cc.i

src/main.s: src/main.cc.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cc.s:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/main.cc.s
.PHONY : src/main.cc.s

src/network.o: src/network.cc.o

.PHONY : src/network.o

# target to build an object file
src/network.cc.o:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/network.cc.o
.PHONY : src/network.cc.o

src/network.i: src/network.cc.i

.PHONY : src/network.i

# target to preprocess a source file
src/network.cc.i:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/network.cc.i
.PHONY : src/network.cc.i

src/network.s: src/network.cc.s

.PHONY : src/network.s

# target to generate assembly for a file
src/network.cc.s:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/network.cc.s
.PHONY : src/network.cc.s

src/response.o: src/response.cc.o

.PHONY : src/response.o

# target to build an object file
src/response.cc.o:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/response.cc.o
.PHONY : src/response.cc.o

src/response.i: src/response.cc.i

.PHONY : src/response.i

# target to preprocess a source file
src/response.cc.i:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/response.cc.i
.PHONY : src/response.cc.i

src/response.s: src/response.cc.s

.PHONY : src/response.s

# target to generate assembly for a file
src/response.cc.s:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/response.cc.s
.PHONY : src/response.cc.s

src/server.o: src/server.cc.o

.PHONY : src/server.o

# target to build an object file
src/server.cc.o:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/server.cc.o
.PHONY : src/server.cc.o

src/server.i: src/server.cc.i

.PHONY : src/server.i

# target to preprocess a source file
src/server.cc.i:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/server.cc.i
.PHONY : src/server.cc.i

src/server.s: src/server.cc.s

.PHONY : src/server.s

# target to generate assembly for a file
src/server.cc.s:
	$(MAKE) -f CMakeFiles/ice.dir/build.make CMakeFiles/ice.dir/src/server.cc.s
.PHONY : src/server.cc.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... ice"
	@echo "... edit_cache"
	@echo "... libs/http_parser.o"
	@echo "... libs/http_parser.i"
	@echo "... libs/http_parser.s"
	@echo "... src/http.o"
	@echo "... src/http.i"
	@echo "... src/http.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/network.o"
	@echo "... src/network.i"
	@echo "... src/network.s"
	@echo "... src/response.o"
	@echo "... src/response.i"
	@echo "... src/response.s"
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

