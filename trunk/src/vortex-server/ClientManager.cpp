/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/

#include "ClientManager.h"
#include "../common/bitBuffer.h"

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
//  BinBitSet set;
//  string buffer;
//  buffer.resize(150);
//  buffer[149]='\0';
  printf("Host:|%s| Port:|%s| Connected.\n",local_socket.getHost().data(),local_socket.getService().data());
//  string html="<h1 style=\"color:red;\">Welcome on my TCPServer!</h1><p>Google is your friend!</p><a href='http://google.com'>Google!</a>";
  char * buffer = new char[32];
  size_t size =32;
  bitBuffer buff;

  while(1)
  {
    cout << "POLL" << endl;
    try
    {
      
      local_socket.recv(buffer,size,0);
      buff = bitBuffer(buffer,size);
      cout <<"Type:"<<(int)buff.read<char>(true)<<endl;
      string str=buff.read<std::string>(true);
      cout << "Str:"<<str<<endl;
      main_event->sendEvent(Event::event.connect.login,str);
    }
    catch(ExSocket * e)
    {
      printf("Client Thread(recv):%s\n",e->what());
      delete e;
    }
  }
  local_socket.free();
}
