/*
 * ChatManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "ChatManager.h"

ChatManager::ChatManager(Event * event)
{
  main_event = event;
  start((void*)this);
}
void ChatManager::execute(void * arg)
{
  ChatManager * pthis = (ChatManager *)arg;
  event_t event;
  while(1)
  {
  	printf("getevst2\n");
  	event = pthis->main_event->getEvent();
  	printf("EVENT recv ChatManager\nTYPE:%d\nDATA:%s\n",event.type,event.data.data());
  }
}
