project(vortex-ee)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
FIND_PACKAGE(OGRE REQUIRED)
FIND_PACKAGE(OIS)

set(CMAKE_CXXFLAGS -Wall -Werror -pedantic -fmessage-length=0)
set(CMAKE_CXXFLAGS_DEBUG -O0 -g3 -c)
set(CMAKE_CXXFLAGS_RELEASE -02)
link_libraries(pthread ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

add_subdirectory(src)

set(srvsrc src/vortex-server)
set(comsrc src/common)
set(clisrc src/vortex-client)
set(bin_dir bin)


include_directories(src/common)
include_directories(${OGRE_INCLUDE_DIR} ${OIS_INCLUDE_DIR})
link_directories($(OGRE_LIB_DIR) ${OIS_LIB_DIR})



set(server_source_files ${srvsrc}/ChatManager.cpp  ${srvsrc}/ClientManager.cpp
                        ${srvsrc}/ConnectionManager.cpp  ${srvsrc}/EventManager.cpp
                        ${srvsrc}/main.cpp  ${srvsrc}/Server.cpp 
                        ${srvsrc}/TCPServer.cpp  ${srvsrc}/WorldManager.cpp)

set(client_source_files ${clisrc}/main.cpp  ${clisrc}/TCPClient.cpp  ${clisrc}/TCPClient.h)

set(common_source_files ${comsrc}/Cond.cpp ${comsrc}/Event.cpp ${comsrc}/Mutex.cpp 
                        ${comsrc}/Packet.cpp ${comsrc}/Semaphore.cpp
                        ${comsrc}/Socket.cpp ${comsrc}/TCPSocket.cpp
                        ${comsrc}/Thread.cpp ${comsrc}/UDPSocket.cpp)

if(WIN32)
  add_definitions(WIN32)
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