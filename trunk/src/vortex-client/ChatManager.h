/*
 * ChatManager.h
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#ifndef CHATMANAGER_H_
#define CHATMANAGER_H_

#include "../common/Thread.h"
#include "../common/Event.h"
class ChatManager: public Thread
{
private:
 Event * main_event;
public:
	ChatManager(Event * event);
	void execute(void * arg);
};

#endif /* CHATMANAGER_H_ */
