/*
 * EventManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "EventManager.h"

EventManager::EventManager(Event * event)
{
	this->event=event;
	start((void *)this);
}
void EventManager::execute(void * arg)
{
  EventManager * pthis = (EventManager *)arg;
  event_t event;
  while(1)
  {
  	event = pthis->event->getEvent();
  	printf("EVENT recv EvenManager\nTYPE:%d\nDATA:%s\n",event.type,event.data.data());
  }
}

