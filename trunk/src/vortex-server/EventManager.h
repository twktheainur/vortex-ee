/*
 * EventManager.h
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include "../common/Thread.h"
#include "../common/Event.h"

class EventManager: public Thread
{
private:
	//This *HAS* to point to a global variable!
  Event * event;
public:
	void execute(void * arg);
	EventManager(Event * event);
};

#endif /* EVENTMANAGER_H_ */
