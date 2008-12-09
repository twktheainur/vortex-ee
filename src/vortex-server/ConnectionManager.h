/*
 * ConnectionManager.h
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_

#include "../../common/Thread.h"
#include "../../common/Event.h"
#include "ClientManager.h"
#include "Server.h"
class Server;
class ConnectionManager: public Thread
{
private:
  Server * serv;
  Event * main_event;
public:
	//Registers a new client in the Server class
  inline Server * getServ(){return serv;}
	void  execute(void * arg);
	void addServerClient(ClientManager * mgr);
	ConnectionManager(Server * serv);
	~ConnectionManager();
};

#endif /* CONNECTIONMANAGER_H_ */
