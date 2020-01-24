cmake_minimum_required(VERSION 2.4)

project(moav2srv)

option(BUILD_HELPERS "Build server helper executables." ON)
option(BUILD_CGI "Build cgi executables." ON)
option(BUILD_CLIENT "Build client executable." ON)
set(GAME_NAME "Mercenaries of Astonia" CACHE STRING "Name of the game.")
set(GAME_HELP "http://localhost/help.html" CACHE STRING "URL for help file.")
set(GAME_NEWS "http://localhost/news.html" CACHE STRING "URL for news.")
set(GAME_HOST "localhost" CACHE STRING "Default hostname/IP of game server.")

add_subdirectory(src)

if(BUILD_HELPERS)
	add_subdirectory(helpers)
endif(BUILD_HELPERS)

if(BUILD_CLIENT)
	add_subdirectory(client)
endif(BUILD_CLIENT)