cmake_minimum_required(VERSION 2.4)

set(MOAV2_GODPWD_SRC
godpwd.cpp
)

set(MOAV2_GODPWD_PLATFORM_SRC )

if(WIN32)
	set(MOAV2_GODPWD_PLATFORM_SRC ${MOAV2_GODPWD_PLATFORM_SRC}
	../src/windows-encrypt.c)
endif(WIN32)

add_executable(godpwd ${MOAV2_GODPWD_SRC} ${MOAV2_GODPWD_PLATFORM_SRC})
