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
#include "../../common/Exception.h"
#include "../../common/TCPSocket.h"
#include "ConnectionManager.h"

#if defined(__WIN32__) || defined(_WIN32)
  #include <winsock2.h>
#endif

enum TCPServerException
{
  E_SOCKET_ERROR,
  E_LISTEN_ERROR,
  E_ACCEPT_ERROR
};
class ConnectionManager;
class TCPServer
{
public:
	TCPServer(int maxClients, string service,ConnectionManager * mgr);
  ~TCPServer();
  void startListener();
private:
	ConnectionManager * mgr;
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
