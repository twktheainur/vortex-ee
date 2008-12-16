/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/

#include "ClientManager.h"
#include "../common/BinBitSet.h"

ClientManager::ClientManager(TCPSocket sock,Event * event)
             :Thread()
{
	main_event=event;
	this->event = new Event();
	socket = sock;
  start((void*)(&sock));
}

ClientManager::~ClientManager()
{
  delete event;
}

void  ClientManager::execute(void * arg)
{
  TCPSocket local_socket = *((TCPSocket*)arg);
  BinBitSet set;
  string buffer;
  buffer.resize(150);
  buffer[149]='\0';
  //printf("%s:%s Connected.",local_socket.getHost().data(),local_socket.getService().data());
  //string html="<h1 style=\"color:red;\">Welcome on my TCPServer!</h1><p>Google is your friend!</p><a href='http://google.com'>Google!</a>";
  set.pushBit(0);
  set.pushBit(1);
  set.pushBit(0);
  set.pushBits<short>(345);

  try
  {
  	size_t len = (set.getByteCount());
     local_socket.send(set.getDataPtr(),&len,0);
  	/*html.resize(200);
    local_socket >> buffer;
    printf("|Recieved Data: %s|\n",buffer.data());
    main_event->sendEvent(EV_INC_CHAT,buffer);
    if(time(NULL)%2==0)
    {
    	printf("Hey!\n");
      local_socket << "AC";
      local_socket << html;
    }
    else
    {
      local_socket << "RJ";
    }*/
  }
  catch(ExSocket * e)
  {
    printf("Client Thread(recv):%s\n",e->what());
    delete e;
  }
  local_socket.free();
}
