cmake_minimum_required(VERSION 3.14)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(MOAV2_SRC
area.c
build.c
ccp_driver.c
cityattack.c
driver.c
driver_etc.c
driver_generic.c
helper.c
lab9.c
look_driver.c
npc_malte.c
path.c
populate.c
server.c
skill.c
skill_driver.c
stunrun.c
svr_act.c
svr_disk.c
svr_do.c
svr_effect.c
svr_glob.c
svr_god.c
svr_tick.c
talk.c
use_driver.c
windows-encrypt.c
windows-gettimeofday.c
windows-mmap.c
)

set(MOAV2_PLATFORM_SRC )

if(WIN32)
	set(MOAV2_PLATFORM_SRC ${MOAV2_PLATFORM_SRC}
	windows-encrypt.c
	windows-gettimeofday.c
	windows-mmap.c)
else(WIN32)
	set(MOAV2_PLATFORM_SRC ${MOAV2_PLATFORM_SRC}
	rdtsc.S)
endif(WIN32)

add_executable(moav2srv ${MOAV2_SRC} ${MOAV2_PLATFORM_SRC} ${MOAV2_INC})
set_property(TARGET moav2srv PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
	target_link_libraries(moav2srv ${ZLIB_LIBRARIES})
endif(ZLIB_FOUND)

if(CMAKE_COMPILER_IS_GNUCC)
	if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
	else(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
		target_link_libraries(moav2srv dl)
	endif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
endif(CMAKE_COMPILER_IS_GNUCC)

if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
	if(CMAKE_COMPILER_IS_GNUCXX)
		target_link_libraries(moav2srv compat)
	endif(CMAKE_COMPILER_IS_GNUCXX)
endif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")

if(WIN32)
	target_link_libraries(moav2srv ws2_32)
	target_link_libraries(moav2srv iphlpapi)
	add_definitions(-D_WIN32_WINNT=0x0501)
	add_definitions(-D_USE_32BIT_TIME_T)
endif(WIN32)

if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
	target_link_libraries(moav2srv xnet)
	target_link_libraries(moav2srv socket)
endif(CMAKE_SYSTEM_NAME STREQUAL "SunOS")

if(BUILD_CGI)
	add_executable(info info.c windows-mmap.c cgilib/cgi-lib.c cgilib/list-lib.c)
	add_executable(acct acct.c skill.c windows-mmap.c cgilib/cgi-lib.c cgilib/list-lib.c)
	add_executable(imgcgi imgcgi.c cgilib/cgi-lib.c cgilib/list-lib.c)
	set_property(TARGET info PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	set_property(TARGET acct PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	set_property(TARGET imgcgi PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	add_definitions(-DIMGCGI)
endif(BUILD_CGI)
