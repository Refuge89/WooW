# Copyright (C) 2014-2016 AscEmu Team <http://www.ascemu.org>

# set up project name
project(lualib C)

# disable warnings
if(MSVC)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W0")
else()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
endif()

set(sources
lapi.c
lauxlib.c
lbaselib.c
lcode.c
ldblib.c
ldebug.c
ldo.c
ldump.c
lfunc.c
lgc.c
linit.c
liolib.c
llex.c
lmathlib.c
lmem.c
loadlib.c
lobject.c
lopcodes.c
loslib.c
lparser.c
lstate.c
lstring.c
lstrlib.c
ltable.c
ltablib.c
ltm.c
lua.c
lundump.c
lvm.c
lzio.c
print.c )

SET( headers
lapi.h
lauxlib.h
lcode.h
ldebug.h
ldo.h
lfunc.h
lgc.h
llex.h
llimits.h
lmem.h
lobject.h
lopcodes.h
lparser.h
lstate.h
lstring.h
ltable.h
ltm.h
lua.h
luaconf.h
lualib.h
lundump.h
lvm.h
lzio.h )

if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
	add_definitions( -DLUA_USE_APICHECK)
endif()

add_library(${PROJECT_NAME} STATIC ${sources} ${headers})

if(IS_64BIT AND NOT MSVC)
   set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPILE_FLAGS "-fPIC")
endif()

unset(sources)
unset(headers)
