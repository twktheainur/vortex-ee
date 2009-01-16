/*
 * EventManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "EventManager.h"
#include "globals.h"

EventManager::EventManager()
{
	this->event=&eventManagerEvent;
	start((void *)this);
}
void EventManager::execute(void * arg)
{
  EventManager * pthis = (EventManager *)arg;
  event_data_t event;
  while(1)
  {
  	event = pthis->event->getEvent();
  	printf("EVENT recv EvenManager\nTYPE:%d\n",event.type/*,event.data.data()*/);
  }
}

