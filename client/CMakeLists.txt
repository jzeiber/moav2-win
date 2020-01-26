cmake_minimum_required(VERSION 3.14)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
	cmake_policy(SET CMP0005 NEW)
endif(COMMAND cmake_policy)

set(CLIENT_SRC
conv.c
dd.c
engine.c
inter.c
main.c
MERC.RC
msvc-dirent.c
options.c
socket.c
sound.c
)

set(CLIENT_PLATFORM_SRC )

if(WIN32)
	set(CLIENT_PLATFORM_SRC ${CLIENT_PLATFORM_SRC}
	)
else(WIN32)
	set(CLIENT_PLATFORM_SRC ${CLIENT_PLATFORM_SRC}
	rdtsc.S)
endif(WIN32)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
add_executable(merc ${CLIENT_SRC} ${CLIENT_PLATFORM_SRC})
set_property(TARGET merc PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
	target_link_libraries(merc ${ZLIB_LIBRARIES})
endif(ZLIB_FOUND)

find_package(PNG REQUIRED)
if(PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIRS})
	target_link_libraries(merc ${PNG_LIBRARIES})
endif(PNG_FOUND)

if(WIN32)
	target_link_libraries(merc ws2_32)
	target_link_libraries(merc iphlpapi)
	target_link_libraries(merc ddraw)
	target_link_libraries(merc dsound)
	add_definitions(-D_WIN32_WINNT=0x0501)
	add_definitions(-D_USE_32BIT_TIME_T)
	add_definitions(-D_MBCS)
	add_definitions(-DCINTERFACE)
	add_definitions(-DNONAMELESSUNION)
endif(WIN32)

add_definitions(-DMNAME="${GAME_NAME}")
add_definitions(-DMHELP="${GAME_HELP}")
add_definitions(-DMNEWS="${GAME_NEWS}")
add_definitions(-DMHOST="${GAME_HOST}")
