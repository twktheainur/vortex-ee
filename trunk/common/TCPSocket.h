/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include "Socket.h"
#if defined(__WIN32__) || defined(_WIN32)
  //#include <ws2tcpip.h>
#endif

#include <string>
using namespace std;

enum TCPSocketExceptions
{
  E_CONNECT_FAIL,
  E_LISTEN_FAIL,
  E_ACCEPT_FAIL
};

class TCPSocket : public Socket
{
public:
  TCPSocket(string host="localhost", string service="",int family=ANY_F ,int backlog=5);
  void connect(struct addrinfo * addr);
  void listen();
  TCPSocket * accept();
private:
  int backlog;
};

class ExTCPSocket : public ExSocket
{
  public:
    ExTCPSocket(int exception,string param="")
    :ExSocket(exception,param)
    {
      switch(exception)
      {
        case E_CONNECT_FAIL:setError("Unable to establish connection");break;
        case E_LISTEN_FAIL:setError("Unable to initiate listening");break;
        case E_ACCEPT_FAIL:setError("Unable to accept incoming connection");break;
      }
    }
};

#endif // TCPSOCKET_H
