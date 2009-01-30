/*
 * ConnectionManagerIn.h
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#ifndef ConnectionManagerIn_H_
#define ConnectionManagerIn_H_
#include "../common/Thread.h"
#include "Client.h"


class Client;
class ConnectionManagerIn: public Thread
{
private:
  Client * cli;
  Event * main_event;
public:
  inline Client * getCli(){return cli;}
  inline void setCli(Client * cli){this->cli=cli;}
	void  execute(void * arg);
	ConnectionManagerIn(Client * cli);
	~ConnectionManagerIn();
};

#endif /* ConnectionManagerIn_H_ */
