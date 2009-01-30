/*
 * ConnectionManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "ConnectionManager.h"
#include "globals.h"
#include "Server.h"

ConnectionManager::ConnectionManager(Server * serv)
{
	this->serv=serv;
	start((void *)this);
}
void ConnectionManager::execute(void * arg)
{
	ConnectionManager * pthis = (ConnectionManager *)arg;
	pthis->serv->setServer(new TCPServer(5,"8080",pthis));
	try
	{
      pthis->serv->getServer()->startListener();
	}
	catch(vortex::Exception * e)
	{
		printf("A client failed to connect:%s\n",e->what());
		delete e;
	}
}
void ConnectionManager::addServerClient(cthreads_t th)
{
	serv->getClients()->push_back(th);
}

ConnectionManager::~ConnectionManager()
{
  //cancel();
}
