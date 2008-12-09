/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#ifndef SERVER_H_
#define SERVER_H_
extern "C"
{
  #include <pthread.h>
}
#include "TCPServer.h"
//#include "ConnectionManager.h"
#include "WorldManager.h"
#include "ChatManager.h"
#include "EventManager.h"
#include "../common/Event.h"
#include <vector>

class ConnectionManager;
class TCPServer;
class Server
{
private:
  ConnectionManager * connectionManagerThread;
  WorldManager * worldManagerThread;
  ChatManager * chatManagerThread;
	EventManager * eventManagerThread;
	Event * eventManagerEvent;
  //Clients will be registered in here by the TCPServer through the ConnectionManager
  vector<ClientManager *> clients;
  TCPServer * server;
public:
	inline vector<ClientManager *>* getClients(){return &clients;}
	inline void setServer(TCPServer * serv){server=serv;}
	inline TCPServer * getServer(){return server;}
	inline Event * getEventManagerEvent(){return eventManagerEvent;}
  Server(Event * event);
  ~Server();
};

#endif /*SERVER_H_*/
