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
  //while(1)
  //{
     //Event::usleep(50000);
     //printf("chat thread loop cycle\n");
  	 //eventManagerEvent.getEvent();
  	//if(event.data[0]=='1')
  	//  printf("Chat EVENT recv ChatManager\nTYPE:%d\nDATA:%s\n",event.type,event.data.data());
 // }
}
