cmake_minimum_required(VERSION 2.4)

project(moav2srv)

option(BUILD_HELPERS "Build server helper executables." ON)
option(BUILD_CGI "Build cgi executables." ON)

add_subdirectory(src)

if(BUILD_HELPERS)
	add_subdirectory(helpers)
endif(BUILD_HELPERS)
