/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include "../common/TCPSocket.h"
#include "../common/Exception.h"
//#include "../common/bitBuffer.h" 
enum TCPServerException
{
  E_SOCKET_ERROR,
  E_CONNECT_ERROR
};

class TCPClient
{
public:
	TCPClient(string host,string port="");
  int send(const void * buffer, size_t * length, int flags);
  int recv(void * buffer, size_t length, int flags);
  void recv(string & str);
  void send(string & str);
  void protocolLoop();
	virtual ~TCPClient();
	inline TCPSocket * getSocket(){return socket;}
private:
  TCPSocket * socket;
};

class ExTCPClient : public vortex::Exception
{
    public:
    ExTCPClient(int exception,string param="")
    :vortex::Exception(exception)
    {
      switch(exception)
      {
        case E_SOCKET_ERROR:setError("Unable to create socket");break;
        case E_CONNECT_ERROR:setError("Unable to connect to the remote host");break;
      }
    }
};

#endif /*TCPCLIENT_H_*/
