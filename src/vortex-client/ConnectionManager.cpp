/*
 * ConnectionManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(Client * cli)
{
	this->cli=cli;
	start((void *)this);
}
void ConnectionManager::execute(void * arg)
{
	ConnectionManager * pthis = (ConnectionManager *)arg;
	try
	{
	  pthis->cli->setClient(new TCPClient("localhost","8080"));
	  pthis->cli->getClient()->protocolLoop();
	}
	catch(Ogre::Exception * e)
	{
		printf("%s\n",e->what());
		delete e;
	}
}

ConnectionManager::~ConnectionManager()
{

}
