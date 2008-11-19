/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include "TCPSocket.h"
#include "Exception.h"

enum TCPServerException
{
  E_SOCKET_ERROR,
  E_LISTEN_ERROR,
  E_ACCEPT_ERROR
};

void default_connection_handler(TCPSocket socket);

class TCPServer
{
public:
	TCPServer(int maxClients, string service,void (*incoming_handler)(TCPSocket)=default_connection_handler);
  ~TCPServer();
  void startListener();
private:
  void (*incoming_handler)(TCPSocket);
  int nClients;
  int maxClients;
  TCPSocket * socket;
};


class ExTCPServer : public Exception
{
    public:
    ExTCPServer(int exception,string param="")
    :Exception(exception)
    {
      switch(exception)
      {
        case E_SOCKET_ERROR:setError("Unable to create socket");break;

      }
    }
};

#endif /*TCPSERVER_H_*/
