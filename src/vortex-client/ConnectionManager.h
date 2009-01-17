/*
 * ConnectionManager.h
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_
#include "../common/Thread.h"
#include "Client.h"


class Client;
class ConnectionManager: public Thread
{
private:
  Client * cli;
  Event * main_event;
public:
  inline Client * getCli(){return cli;}
  inline void setCli(Client * cli){this->cli=cli;}
	void  execute(void * arg);
	ConnectionManager(Client * cli);
	~ConnectionManager();
};

#endif /* CONNECTIONMANAGER_H_ */
