/*
 * ConnectionManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(Server * serv)
{
	this->serv=serv;
	start((void *)this);
}
void ConnectionManager::execute(void * arg)
{
	ConnectionManager * pthis = (ConnectionManager *)arg;
	try
	{
	  pthis->serv->setServer(new TCPServer(5,"8080",pthis));
    pthis->serv->getServer()->startListener();
	}
	catch(vortex::Exception * e)
	{
		printf("%s\n",e->what());
		delete e;
	}
}
void ConnectionManager::addServerClient(ClientManager * mgr)
{
	serv->getClients()->push_back(mgr);
}

ConnectionManager::~ConnectionManager()
{
  //cancel();
}
