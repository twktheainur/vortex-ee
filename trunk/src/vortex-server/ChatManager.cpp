/*
 * ChatManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "ChatManager.h"
#include "globals.h"

ChatManager::ChatManager()
{
  start((void*)this);
}
void ChatManager::execute(void * arg)
{
  ChatManager * pthis = (ChatManager *)arg;
  event_data_t event;
  while(1)
  {
  	//event = eventManagerEvent.getEvent();
  	//if(event.data[0]=='1')
  	//  printf("Chat EVENT recv ChatManager\nTYPE:%d\nDATA:%s\n",event.type,event.data.data());
  }
}
