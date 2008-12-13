/*
 * ChatManager.h
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#ifndef APPLICATIONMANAGER_H_
#define APPLICATIONMANAGER_H_

#include "../common/Thread.h"
#include "../common/Event.h"
#include "Application.h"
class ApplicationManager: public Thread
{
private:
 Event * main_event;
 Application * application;
public:
	ApplicationManager(Event * event);
	void execute(void * arg);
};

#endif
