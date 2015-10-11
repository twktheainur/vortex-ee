# Introduction #

Cette page vous explique comment compiler et coder pour Vortex avec l'outil CMake.

# Détails #

**Prérequis:** Avoir la dernière version du code; pour cela faites un svn {checkout|update} selon votre situation (pour plus de détails: http://code.google.com/p/vortex-ee/source/checkout ).

**Explication:** En lui même, CMake ne compile pas Vortex: il créé un fichier qui permettra de le compiler par la suite. Ce fichier peut être un fichier de projet pour un IDE, un makefile, etc.

**Etape un: Générer le makefile** Nous allons générer un makefile, étant donné que c'est l'option la plus utilisée. Pour cela, on se place dans le répertoire vortex-ee, et on lance CMake. Ce dernier a besoin d'un fichier CMakeLists.txt, qui se trouve à la racine de vortex-ee. On tape donc:

```
gardaud@euler:~/vortex-ee$ cmake .
```

Ce qui devrait donner quelque chose comme:

```
-- The C compiler identification is GNU
-- The CXX compiler identification is GNU
-- Check for working C compiler: /usr/bin/gcc
-- Check for working C compiler: /usr/bin/gcc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- checking for one of the modules 'OGRE'
--   found OGRE, version 1.6.0
--   libraries : OgreMain from /usr/local/lib
--   includes  : /usr/local/include;/usr/local/include/OGRE
-- checking for one of the modules 'OIS'
--   found OIS, version 1.0.0
--   libraries : OIS from /usr/lib
--   includes  : /usr/include/OIS
-- Configuring done
-- Generating done
-- Build files have been written to: /home/gardaud/vortex-ee
```

Nous obtenons un Makefile dans le même répertoire. Il nous suffit ensuite de lancer make:

```
gardaud@euler:~/vortex-ee$ make
Linking CXX executable bin/vortex-client
[ 39%] Built target bin/vortex-client
Scanning dependencies of target vortex-server
[ 42%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/vortex-serverChatManager.o
[ 46%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/vortex-server/ClientManager.o
[ 50%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/vortex-server/ConnectionManager.o
[ 53%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/vortex-server/EventManager.o
[ 57%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/vortex-server/main.o
[ 60%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/vortex-server/Server.o
[ 64%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/vortex-server/TCPServer.o
[ 67%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/vortex-server/WorldManager.o
[ 71%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/common/Cond.o
/home/gardaud/vortex-ee/src/common/Cond.cpp: In member function ‘void Cond::wait(bool)’:
/home/gardaud/vortex-ee/src/common/Cond.cpp:21: warning: taking address of temporary
[ 75%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/common/Event.o
[ 78%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/common/Mutex.o
[ 82%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/common/Packet.o
[ 85%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/common/Semaphore.o
[ 89%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/common/Socket.o
[ 92%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/common/TCPSocket.o
[ 96%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/common/Thread.o
[100%] Building CXX object CMakeFiles/bin/vortex-server.dir/src/common/UDPSocket.o
Linking CXX executable bin/vortex-server
[100%] Built target bin/vortex-server
```

Et voilà, tout est bon, les exécutables se trouvent dans bin/ .