/*
 * ChatManager.h
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#ifndef APPLICATIONMANAGER_H_
#define APPLICATIONMANAGER_H_

#include "../common/Thread.h"
#include "Application.h"

class Client;
class ApplicationManager: public Thread
{
private:
 Application * application;
 Client * cli;
public:
	ApplicationManager(Client * cli);
	void execute(void * arg);
};

#endif
