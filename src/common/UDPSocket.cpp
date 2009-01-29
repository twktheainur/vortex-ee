/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#include "UDPSocket.h"

UDPSocket::UDPSocket (string host, string service)
          :Socket(host,service,ANY_F,UDP_SOCK){}

 void UDPSocket::setPeer(string host, string service)
 {
   initInfo(host,service,ANY_F,UDP_SOCK);
 }

int UDPSocket::send(const void * buffer, size_t * length, int flags )
{
  size_t total = 0;
  size_t bytesleft = *length;
  int n=0;
  while(total<*length)
  {
     if((n=::sendto(getSockFD(),(char *)buffer+total,bytesleft,flags,
                    getInfo()->ai_addr,getInfo()->ai_addrlen))==SOCK_ERR){break;}
     total +=n;
     bytesleft-=n;
  }
  *length = total;
  if(n==-1)
    throw new ExUDPSocket(E_SENDTO_FAIL);
  return total;
}
int UDPSocket::recv(void * buffer, size_t length, int flags )
{
  int recv_ret;
  struct sockaddr_storage remote_addr;
   socklen_t sin_len = sizeof remote_addr;

  #ifdef WIN32
  if(SOCK_ERR==(recv_ret=::recvfrom(getSockFD(),(char *)buffer,length,flags,
                           (struct sockaddr*)&remote_addr,&sin_len)))
  #else
  if(!(recv_ret=::recvfrom(getSockFD(),(void *)buffer,length,flags,
                           (struct sockaddr*)&remote_addr,&sin_len)))
  #endif
    throw new ExUDPSocket(E_RECVFROM_FAIL);
    setPeer((struct addrinfo *)&remote_addr);
  return recv_ret;
}
