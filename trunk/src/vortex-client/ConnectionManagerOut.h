/*
 * ConnectionManagerOut.h
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#ifndef ConnectionManagerOut_H_
#define ConnectionManagerOut_H_
#include "../common/Thread.h"
#include "Client.h"


class Client;
class ConnectionManagerOut: public Thread
{
private:
  Client * cli;
  Event * main_event;
public:
  inline Client * getCli(){return cli;}
  inline void setCli(Client * cli){this->cli=cli;}
	void  execute(void * arg);
	ConnectionManagerOut(Client * cli);
	~ConnectionManagerOut();
};

#endif /* ConnectionManagerOut_H_ */
