# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_SOURCE_DIR = /home/nolan/workspace/program/DaChun/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nolan/workspace/program/DaChun/src

# Include any dependencies generated for this target.
include server/tests/CMakeFiles/HTTPClient.dir/depend.make

# Include the progress variables for this target.
include server/tests/CMakeFiles/HTTPClient.dir/progress.make

# Include the compile flags for this target's objects.
include server/tests/CMakeFiles/HTTPClient.dir/flags.make

server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.o: server/tests/CMakeFiles/HTTPClient.dir/flags.make
server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.o: server/tests/http_client_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nolan/workspace/program/DaChun/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.o"
	cd /home/nolan/workspace/program/DaChun/src/server/tests && g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HTTPClient.dir/http_client_test.cpp.o -c /home/nolan/workspace/program/DaChun/src/server/tests/http_client_test.cpp

server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HTTPClient.dir/http_client_test.cpp.i"
	cd /home/nolan/workspace/program/DaChun/src/server/tests && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nolan/workspace/program/DaChun/src/server/tests/http_client_test.cpp > CMakeFiles/HTTPClient.dir/http_client_test.cpp.i

server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HTTPClient.dir/http_client_test.cpp.s"
	cd /home/nolan/workspace/program/DaChun/src/server/tests && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nolan/workspace/program/DaChun/src/server/tests/http_client_test.cpp -o CMakeFiles/HTTPClient.dir/http_client_test.cpp.s

server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.o.requires:

.PHONY : server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.o.requires

server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.o.provides: server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.o.requires
	$(MAKE) -f server/tests/CMakeFiles/HTTPClient.dir/build.make server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.o.provides.build
.PHONY : server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.o.provides

server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.o.provides.build: server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.o


# Object files for target HTTPClient
HTTPClient_OBJECTS = \
"CMakeFiles/HTTPClient.dir/http_client_test.cpp.o"

# External object files for target HTTPClient
HTTPClient_EXTERNAL_OBJECTS =

server/tests/HTTPClient: server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.o
server/tests/HTTPClient: server/tests/CMakeFiles/HTTPClient.dir/build.make
server/tests/HTTPClient: server/tests/CMakeFiles/HTTPClient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nolan/workspace/program/DaChun/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable HTTPClient"
	cd /home/nolan/workspace/program/DaChun/src/server/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HTTPClient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
server/tests/CMakeFiles/HTTPClient.dir/build: server/tests/HTTPClient

.PHONY : server/tests/CMakeFiles/HTTPClient.dir/build

server/tests/CMakeFiles/HTTPClient.dir/requires: server/tests/CMakeFiles/HTTPClient.dir/http_client_test.cpp.o.requires

.PHONY : server/tests/CMakeFiles/HTTPClient.dir/requires

server/tests/CMakeFiles/HTTPClient.dir/clean:
	cd /home/nolan/workspace/program/DaChun/src/server/tests && $(CMAKE_COMMAND) -P CMakeFiles/HTTPClient.dir/cmake_clean.cmake
.PHONY : server/tests/CMakeFiles/HTTPClient.dir/clean

server/tests/CMakeFiles/HTTPClient.dir/depend:
	cd /home/nolan/workspace/program/DaChun/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nolan/workspace/program/DaChun/src /home/nolan/workspace/program/DaChun/src/server/tests /home/nolan/workspace/program/DaChun/src /home/nolan/workspace/program/DaChun/src/server/tests /home/nolan/workspace/program/DaChun/src/server/tests/CMakeFiles/HTTPClient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : server/tests/CMakeFiles/HTTPClient.dir/depend
