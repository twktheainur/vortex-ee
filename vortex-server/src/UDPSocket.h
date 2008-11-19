/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/

#ifndef UDPSOCKET_H
#define UDPSOCKET_H
#include "Socket.h"

#include <string>
using namespace std;

enum UDPSocketExceptions
{
  E_SENDTO_FAIL,
  E_RECVFROM_FAIL
};

class UDPSocket :public Socket
{
public:
  UDPSocket(string host="", string service="");
  int send (const void * buffer, size_t * length, int flags);
  int recv (void * buffer, size_t length, int flags);
  void setPeer(string host, string service);
  void setPeer(struct addrinfo * peer){this->peer=peer;}
  struct addrinfo * getPeer(){return peer;}

private:
  struct addrinfo * peer;
};

class ExUDPSocket: public ExSocket
{
  public:
  ExUDPSocket(int exception, string param="")
  :ExSocket(exception,param)
  {
    switch(exception)
    {
      case E_SENDTO_FAIL:setError("Unable to send data to destination");break;
      case E_RECVFROM_FAIL:setError("Unable to recieve sata from source");break;
    }
  }
};

#endif // UDPSOCKET_H
