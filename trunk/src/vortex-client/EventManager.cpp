/*
 * EventManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "EventManager.h"

EventManager::EventManager()
{
	start((void *)this);
}
void EventManager::execute(void * arg)
{
  EventManager * pthis = (EventManager *)arg;
  event_data_t event;
  while(1)
  {
  	event = worldManagerEvent.getEvent();
  	printf("EVENT recv EvenManager\nTYPE:%d\nDATA:%s\n",event.type/*,event.data.data()*/);
  }
}

