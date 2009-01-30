/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
/*CLASS Server: Spawns all the threads of the application and starts the TCPServer*/
#ifndef SERVER_H_
#define SERVER_H_
extern "C"
{
  #include <pthread.h>
}
#include "TCPServer.h"
#include "WorldManager.h"
#include "../common/Event.h"
#include "ClientManagerIn.h"
#include "ClientManagerOut.h"
#include <vector>

class ConnectionManager;
class TCPServer;

typedef struct cthreads
{
  ClientManagerIn * in;
  ClientManagerOut * out;
}cthreads_t;

class Server
{
private:
	//Thread Classes
  ConnectionManager * connectionManagerThread;
  WorldManager * worldManagerThread;
  //Clients will be registered in here by the TCPServer through the ConnectionManager
  vector<cthreads_t> clients;
  TCPServer * server;
public:
	inline vector<cthreads_t>* getClients(){return &clients;}
	inline void setServer(TCPServer * serv){server=serv;}
	inline TCPServer * getServer(){return server;}
	//Here event has to point to a global scope variable sharable by threads
  Server();
  ~Server();
};

#endif /*SERVER_H_*/
