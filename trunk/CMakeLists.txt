project(vortex-ee)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
FIND_PACKAGE(OGRE REQUIRED)
FIND_PACKAGE(OIS)

if(WIN32)
  set(PTHREADS_LIBRARY c:/Pre-built.2/lib/pthreadGC2.dll)
  set(PTHREADS_INCLUDE_DIR c:/Pre-built.2/include)
  set(SOCKET_LIBRARY C:/mingw/lib/libwsock32.a C:/mingw/lib/libws2_32.a)
  set(SOCKET_INCLUDE C:/mingw/include)
endif(WIN32)

set(CMAKE_CXXFLAGS -Wall -Werror -pedantic -fmessage-length=0 -Wno-deprecated)
set(CMAKE_CXXFLAGS_DEBUG -O0 -g3 -c)
set(CMAKE_CXXFLAGS_RELEASE -02)
link_libraries(pthread ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${PTHREADS_LIBRARY} ${SOCKET_LIBRARY})

add_subdirectory(src)

set(srvsrc src/vortex-server)
set(comsrc src/common)
set(clisrc src/vortex-client)
set(bin_dir bin)


include_directories(src/common)
include_directories(${OGRE_INCLUDE_DIR} ${OIS_INCLUDE_DIR} ${PTHREADS_INCLUDE_DIR} ${SOCKET_INCLUDE})
link_directories($(OGRE_LIB_DIR) ${OIS_LIB_DIR})



set(server_source_files ${srvsrc}/ChatManager.cpp  ${srvsrc}/ClientManager.cpp
                        ${srvsrc}/ConnectionManager.cpp  ${srvsrc}/EventManager.cpp
                        ${srvsrc}/main.cpp  ${srvsrc}/Server.cpp 
                        ${srvsrc}/TCPServer.cpp)

set(client_source_files ${clisrc}/main.cpp ${clisrc}/TCPClient.cpp
			${clisrc}/Application.cpp ${clisrc}/VortexFrameListener.cpp
			${clisrc}/InputManager.cpp
			${clisrc}/ChatManager.cpp  ${clisrc}/ConnectionManager.cpp  
			${clisrc}/EventManager.cpp ${clisrc}/ApplicationManager.cpp
                        ${clisrc}/Client.cpp)

set(common_source_files ${comsrc}/Cond.cpp ${comsrc}/Event.cpp ${comsrc}/Mutex.cpp 
                        ${comsrc}/Packet.cpp ${comsrc}/Semaphore.cpp
                        ${comsrc}/Socket.cpp ${comsrc}/TCPSocket.cpp
                        ${comsrc}/Thread.cpp ${comsrc}/UDPSocket.cpp 
                        ${comsrc}/WorldManager.cpp ${comsrc}/BinBitSet.cpp)

if(WIN32)
  add_definitions(-DWIN32)
elseif(APPLE)
  add_definitions(APPLE)
endif(WIN32)
                        
                        
add_executable(bin/vortex-server ${server_source_files} ${common_source_files})
add_executable(bin/vortex-client ${client_source_files} ${common_source_files})

if(WIN32)
  add_custom_command(TARGET target mv bin/plugins_win.cfg bin/plugins.cfg)
else(WIN32)
  add_custom_command(TARGET target mv bin/plugins_uni.cfg bin/plugins.cfg)
endif(WIN32)
