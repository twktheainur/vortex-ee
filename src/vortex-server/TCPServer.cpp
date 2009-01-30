/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#include "TCPServer.h"
#include "ClientManagerOut.h"
#include "Server.h"

TCPServer::TCPServer(int maxClients,string service,ConnectionManager * mgr)
{
  this->mgr=mgr;
  nClients=0;
  this->maxClients = maxClients;
  socket = new TCPSocket("",service,ANY_F,maxClients);
  struct addrinfo * p=NULL;
  p = socket->getInfo();
  for(;p!=NULL;p=p->ai_next)
  {
    try
    {
      int yes=1;
      socket->socket(p);
      socket->setSockOpt(SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes);
      socket->setSockOpt(IPPROTO_TCP,TCP_NODELAY ,&yes,sizeof yes);
      //socket->setNblock();
      socket->bind(p);
      break;
    }
    catch(exception * e)
    {
      printf("TCPServer:%s\n",e->what());
      delete e;
    }
  }
  if(p==NULL)
    throw new ExTCPServer(E_SOCKET_ERROR);
  socket->setInfo(p);
}

TCPServer::~TCPServer()
{
  socket->free();
	delete socket;
}

void TCPServer::startListener()
{
  TCPSocket * new_socket;
  try
  {
    socket->listen();
  }
  catch (exception * e)
  {
    printf("TCPServer::startListener()(listen):%s\n",e->what());
    delete e;
    throw new ExTCPServer(E_LISTEN_ERROR);
  }

  while(1)
  {
    try
    {
      new_socket = socket->accept();
      try
      {
        ClientManagerOut * cmo = new ClientManagerOut(*new_socket);
        struct cthreads ct = {new ClientManagerIn(*new_socket,cmo),cmo};
        mgr->addServerClient(ct);
        delete new_socket;
      }
      catch(vortex::Exception * e)
      {
        cout << "TCPServer::startListener()(incoming_handler):%s\n" << e->what() << endl;
        delete e;
      }
    }
    catch (vortex::Exception * e)
    {
        printf("Polling: No new clients:%s\n",e->what());
        Event::usleep(10000);
      delete e;
    }
  }
}

