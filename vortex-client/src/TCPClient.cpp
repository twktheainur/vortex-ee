/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#include "TCPClient.h"

TCPClient::TCPClient(string host,string service)
{
  struct addrinfo * p;
  socket = new TCPSocket("",service,ANY_F);
  for(p = socket->getInfo();p!=NULL;p=p->ai_next)
  {
    try
    {
      socket->socket(p);
      socket->connect(p);
      break;
    }
    catch(ExTCPSocket * e)
    {
      printf("TCPClient:%s\n",e->what());
      throw new ExTCPClient(E_CONNECT_ERROR,e->what());
      delete e;
    }
  }
  if(p==NULL)
    throw new ExTCPClient(E_SOCKET_ERROR);
  socket->setInfo(p);
}


TCPClient::~TCPClient()
{
  delete socket;
}


int TCPClient::send(const void * buffer, size_t * length, int flags)
{
  return socket->send(buffer,length,flags);
}
int TCPClient::recv(void * buffer, size_t length, int flags)
{
  return socket->recv(buffer,length,flags);
}
