# Copyright (C) 2014-2016 AscEmu Team <http://www.ascemu.org>

# set up our project name
project(shared CXX)

set(sources
    CThreads.cpp 
    CrashHandler.cpp 
    Log.cpp 
    MersenneTwister.cpp 
    printStackTrace.cpp 
    Util.cpp 
    crc32.cpp 
    MapFile.cpp
    MapFileEntry.cpp
    TextFile.cpp
    FindFiles.cpp
    DynLib.cpp
    SysInfo.cpp
    PerformanceCounter.cpp
    Threading/Mutex.cpp
    Threading/Threading.h  
    Threading/ThreadPool.cpp 
    Threading/AtomicULong.cpp 
    Threading/AtomicFloat.cpp 
    Threading/AtomicCounter.cpp 
    Threading/AtomicBoolean.cpp 
    Threading/ConditionVariable.cpp
    ascemu_getopt.cpp 
    Auth/BigNumber.cpp 
    Auth/MD5.cpp 
    Auth/Sha1.cpp 
    Auth/WowCrypt.cpp 
    Config/Config.cpp 
    Database/Database.cpp 
    Database/MySQLDatabase.cpp 
    Database/CreateInterface.cpp 
    Network/CircularBuffer.cpp 
    Network/Socket.cpp
	)
	
set(headers 
	ascemu_getopt.h
	ascemuConfig.h
	Array.h
	AuthCodes.h
	ByteBuffer.h
	CallBack.h
	CircularQueue.h
	Common.h
    CommonTypes.hpp
	CrashHandler.h
	crc32.h
	CRefcounter.h
	CThreads.h
	Database/DatabaseEnv.h
	DynLib.hpp
	Errors.h
	FastQueue.h
	FindFiles.hpp
	FindFilesResult.hpp
	Timer.h
	LocationVector.h
	Log.h
	MapFile.h
	MapFileEntry.h
	MersenneTwister.h
	PerformanceCounter.hpp
	PreallocatedQueue.h
	printStackTrace.h
	Range.h
	RC4Engine.h
	Singleton.h
	StackBuffer.h
	StackWalker.h
	StackTrace.h
	Storage.h
	SysInfo.hpp
	TextFile.h
	TLSObject.h
	Tokenizer.h
	Util.h
	WorldPacket.h
	WoWGuid.h
	Auth/BigNumber.h
	Auth/MD5.h
	Auth/Sha1.h
	Auth/WowCrypt.h
	Config/Config.h
	Config/ConfigEnv.h
	Database/Database.h
	Database/DataStore.h
	Database/Field.h
	Database/MySQLDatabase.h
	Network/CircularBuffer.h
	Network/ListenSocketFreeBSD.h
	Network/ListenSocketLinux.h
	Network/ListenSocketWin32.h
	Network/Network.h
	Network/Socket.h
	Network/SocketMgrFreeBSD.h
	Network/SocketMgrLinux.h
	Network/SocketMgrWin32.h
	Network/SocketOps.h
	Network/SocketDefines.h
	Threading/AtomicBoolean.h
	Threading/AtomicCounter.h
	Threading/AtomicFloat.h
	Threading/AtomicULong.h
	Threading/ConditionVariable.h
	Threading/Guard.h
	Threading/LockedQueue.h
	Threading/Mutex.h
	Threading/Queue.h
	Threading/RWLock.h
	Threading/Threading.h
	Threading/ThreadPool.h
	Threading/ThreadStarter.h
)

if(WIN32)
   set(sources ${sources} Network/SocketMgrWin32.cpp Network/SocketOpsWin32.cpp Network/SocketWin32.cpp StackWalker.cpp StackTrace.cpp)
elseif((CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "kFreeBSD") OR APPLE)
   set(sources ${sources} Network/SocketMgrFreeBSD.cpp Network/SocketFreeBSD.cpp Network/SocketOpsFreeBSD.cpp)
else()
   set(sources ${sources} Network/SocketMgrLinux.cpp Network/SocketLinux.cpp Network/SocketOpsLinux.cpp)
endif()

include_directories(
    ${OPENSSL_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS}
    ${MYSQL_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(${PROJECT_NAME} STATIC ${sources} ${headers})
target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MYSQL_LIBRARIES} ${OPENSSL_LIBRARIES} ${EXTRA_LIBS})

unset(sources)
unset(headers)
