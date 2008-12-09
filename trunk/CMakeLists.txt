project(vortex-ee)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXXFLAGS -Wall -Werror -pedantic -fmessage-length=0)
set(CMAKE_CXXFLAGS_DEBUG -O0 -g3 -c)
set(CMAKE_CXXFLAGS_RELEASE -02)
link_libraries(pthread)

add_subdirectory(src)

set(srvsrc src/vortex-server)
set(comsrc src/common)
set(clisrc src/vortex-client)


include_directories(src/common)



set(server_source_files ${srvsrc}/ChatManager.cpp  ${srvsrc}/ClientManager.cpp
                        ${srvsrc}/ConnectionManager.cpp  ${srvsrc}/EventManager.cpp
                        ${srvsrc}/main.cpp  ${srvsrc}/Server.cpp 
                        ${srvsrc}/TCPServer.cpp  ${srvsrc}/WorldManager.cpp)

set(client_source_files ${clisrc}/main.cpp  ${clisrc}/TCPClient.cpp  ${clisrc}/TCPClient.h)

set(common_source_files ${comsrc}/Cond.cpp ${comsrc}/Event.cpp ${comsrc}/Mutex.cpp 
                        ${comsrc}/Packet.cpp ${comsrc}/Semaphore.cpp
                        ${comsrc}/Socket.cpp ${comsrc}/TCPSocket.cpp
                        ${comsrc}/Thread.cpp ${comsrc}/UDPSocket.cpp)

                        
                        
add_executable(vortex-server ${server_source_files} ${common_source_files})
add_executable(vortex-client ${client_source_files} ${common_source_files})


