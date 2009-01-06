/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#include "TCPSocket.h"
#if defined(__WIN32__) || defined(_WIN32)
  //#include <ws2tcpip.h>
#endif
/******************************************************************************/
#ifdef _WIN32
const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt)
{
        if (af == AF_INET)
        {
                struct sockaddr_in in;
                memset(&in, 0, sizeof(in));
                in.sin_family = AF_INET;
                memcpy(&in.sin_addr, src, sizeof(struct in_addr));
                getnameinfo((struct sockaddr *)&in, sizeof(struct sockaddr_in), dst, cnt, NULL, 0, NI_NUMERICHOST);
                return dst;
        }
        else if (af == AF_INET6)
        {
                struct sockaddr_in6 in;
                memset(&in, 0, sizeof(in));
                in.sin6_family = AF_INET6;
                memcpy(&in.sin6_addr, src, sizeof(struct in_addr6));
                getnameinfo((struct sockaddr *)&in, sizeof(struct
sockaddr_in6), dst, cnt, NULL, 0, NI_NUMERICHOST);
                return dst;
        }
        return NULL;
}
int inet_pton(int af, const char *src, void *dst)
{
        struct addrinfo hints, *res, *ressave;
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = af;
        if (getaddrinfo(src, NULL, &hints, &res) != 0)
        {
          //dolog(LOG_ERR, "Couldn't resolve host %s\n", src);
          return -1;
        }
        ressave = res;
        while (res)
        {
          memcpy(dst, res->ai_addr, res->ai_addrlen);
                res = res->ai_next;
        }
        freeaddrinfo(ressave);
        return 0;
}
#endif
/***************************Constructor/Destructor*****************************/
TCPSocket::TCPSocket(string host, string service,int family,int backlog)
          :Socket(host,service,family,TCP_SOCK)
{
  this->backlog = backlog;
}
/******************************************************************************/
/**************************Member Methods**************************************/
/*----------------------------------------------------------------------------*/

 void TCPSocket::connect(struct addrinfo * addr)
 {
     if(::connect(getSockFD(),addr->ai_addr,addr->ai_addrlen)==-1)
     {
        printf("TCPSocket.cpp:15 -- THROWING E_CONNECT_FAIL\n");
        throw new ExTCPSocket(E_CONNECT_FAIL);
     }
 }
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
 void TCPSocket::listen()
 {
     if(::listen(getSockFD(),backlog)==-1)
     {
       printf("TCPSocket.cpp:22 -- THROWING E_LISTEN_FAIL\n");
       throw new ExTCPSocket(E_LISTEN_FAIL);
     }
 }
 /*----------------------------------------------------------------------------*/
 /*----------------------------------------------------------------------------*/
 TCPSocket * TCPSocket::accept()
 {
   int new_fd;
   struct sockaddr_storage remote_addr;
   socklen_t sin_len = sizeof remote_addr;
   string host;
   string port;

   TCPSocket * new_sock;
   stringstream ConvertStream;
   new_fd = ::accept(getSockFD(),
                     (struct sockaddr *)&remote_addr,
                     (socklen_t *)&sin_len);
     if(new_fd==-1)
     {
       printf("TCPSocket.cpp:37 -- THROWING E_ACCEPT_FAIL\n");
       throw new ExTCPSocket(E_ACCEPT_FAIL);
     }
   host.resize(INET6_ADDRSTRLEN);
   inet_ntop(remote_addr.ss_family,
             get_in_addr((struct sockaddr *)&remote_addr),
             (char *)host.data(), host.size());
   ConvertStream << get_in_port((struct sockaddr*)&remote_addr);
   ConvertStream >> port;
   new_sock = new TCPSocket(host,port,get_in_fam((struct sockaddr*)&remote_addr),0);
   new_sock->setSockFD(new_fd);
   return new_sock;
 }
 /*----------------------------------------------------------------------------*/

