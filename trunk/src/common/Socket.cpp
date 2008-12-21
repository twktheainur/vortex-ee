/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#include "Socket.h"
/***********************Constructor / Destructor*******************************/
/*----------------------------Socket Constructor------------------------------*/
Socket::Socket (string host, string service, int family, int type)
{
  #if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
 // Must be done at the beginning of every WinSock program
WSADATA w;    // used to store information about WinSock version
int error = WSAStartup (0x0202, &w);   // Fill in w

if (error)
{ // there was an error
  return;
}
if (w.wVersion != 0x0202)
{ // wrong WinSock version!
  WSACleanup (); // unload ws2_32.dll
  return;
}
  #endif
    initInfo(host,service,family,type);
}
/*----------------------------------------------------------------------------*/
/*------------------------------Socket Destructor-----------------------------*/
Socket::~Socket()
{
  #if defined(__WIN32__) || defined(_WIN32)
    WSACleanup();
  #endif
}
/*----------------------------------------------------------------------------*/
/******************************************************************************/
/****************************Member METHODS************************************/
/*----------------------------------------------------------------------------*/
 void Socket::initInfo(string host, string service, int family, int type)
 {
    int gair;
    struct addrinfo hints;
    //char err[40];
    const char * passed_host=host.data();
    memset(&hints, 0, sizeof hints);
    hints.ai_family = family;
    hints.ai_socktype = type;
    this->service = service;
    if(host=="")
    {
      this->host="localhost";
      hints.ai_flags=AI_PASSIVE;
      passed_host=NULL;
    }
    else
      this->host=host;
    if((gair = getaddrinfo(passed_host,service.data(),&hints,&info)))
    {
      printf("Socket.cpp:49 -- THROWING E_GETADDRINFO_FAIL\n");
      throw new ExSocket(E_GETADDRINFO_FAIL,strerror(errno));
    }

 }
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
 void Socket::free()
 {
	 if(close(sockFD)==-1 && errno!=EBADF)
	   {
	     printf("Socket.cpp:70 -- THROWING E_CLOSE_FAIL\n");
	     throw new ExSocket(E_CLOSE_FAIL);
	   }
	   freeaddrinfo(info);
	   info=NULL;
 }
/*----------------------------------------------------------------------------*/
int Socket::bind(struct addrinfo * addr)
{
  int bind_ret;
  char port[6];
  sprintf(port,"%d",get_in_port((struct sockaddr *)addr));
  if((bind_ret = ::bind(sockFD,addr->ai_addr,addr->ai_addrlen)))
  {
    printf("Socket.cpp:64 -- THROWING E_BIND_FAIL\n");
    throw new ExSocket(E_BIND_FAIL,port);
  }
  return bind_ret;
}

/*----------------------------------------------------------------------------*/
/*-----------------------Open the socket--------------------------------------*/
void Socket::socket(struct addrinfo * addr)
{
    if((sockFD = ::socket(addr->ai_family,addr->ai_socktype,addr->ai_protocol))==-1)
    {
      printf("Socket.cpp:76 -- THROWING E_SOCKET_FAIL\n");
      throw new ExSocket(E_SOCKET_FAIL);
    }
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void Socket::setSockOpt(int level, int optname, void * optval, size_t size)
{
  if(setsockopt(sockFD,level,optname,optval,size)==-1)
  {
    printf("Socket.cpp:86 -- THROWING E_SETSOCKOPT_FAIL\n");
    throw new ExSocket(E_SETSOCKOPT_FAIL);
  }
}
/*----------------------------------------------------------------------------*/
/*-------send: Send data over the socket--------------------------------------*/
int Socket::send(const void * buffer, size_t * length, int flags)
{
  size_t total = 0;
  size_t bytesleft = *length;
  int n;
  while(total<*length)
  {
     n=::send(sockFD,(char *)buffer+total,bytesleft,flags);
     if(n==-1){break;}
     total +=n;
     bytesleft-=n;
  }
  *length = total;
  if(n==-1)
  {
    printf("Socket.cpp:125 -- THROWING E_SEND_FAIL\n");
    throw new ExSocket(E_SEND_FAIL,strerror(errno));
  }
  return total;
}
/*----------------------------------------------------------------------------*/
/*-----------recv: Get data from the socket.----------------------------------*/
int Socket::recv(void * buffer, size_t length, int flags)
{
  int recv_ret;
  if(!(recv_ret=::recv(sockFD,buffer,length,flags)))
  {
    printf("Socket.cpp:26 -- THROWING E_RECV_FAIL\n");
    throw new ExSocket(E_RECV_FAIL);
  }
  return recv_ret;
}
/*----------------------------------------------------------------------------*/
int Socket::poll_read()
{
	int ret;
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(sockFD,&fds);
  if((ret=(select(sockFD+1,&fds,NULL,NULL,NULL)))==-1)
    throw new ExSocket(E_POLL_FAIL);
  return ret;
}
/*----------------------------------------------------------------------------*/
/*---------------------------Stream Operators Definition----------------------*/
Socket& operator<<(Socket& sock,std::basic_string<char, std::char_traits<char>, std::allocator<char> > data)
{
  try
  {
    size_t length = data.size();
    sock.send(data.data(),&length,0);
  }
  catch(ExSocket e)
  {
    throw;
  }
  return sock;
}
Socket& operator>>(Socket& sock,std::basic_string<char, std::char_traits<char>, std::allocator<char> >& data)
{
  try
  {
    size_t length = data.size();
    sock.recv((char *)data.data(),length,0);
  }
  catch(ExSocket e)
  {
    throw;
  }
  return sock;
}
/*------------------------SockFD getter---------------------------------------*/
int Socket::getSockFD()
  {
    return sockFD;
  }
/*----------------------------------------------------------------------------*/
/*---------------------------Host setter--------------------------------------*/
  void Socket::setHost(string new_var)
  {
    host = new_var;
  }
/*----------------------------------------------------------------------------*/
/*------------------------Host getter-----------------------------------------*/
  string Socket::getHost()
  {
    return host;
  }
/*----------------------------------------------------------------------------*/
/*--------------------------Service setter------------------------------------*/
  void Socket::setService(string new_var)
  {
    service = new_var;
  }
/*----------------------------------------------------------------------------*/
/*---------------------------Service Getter-----------------------------------*/
  string Socket::getService()
  {
    return service;
  }
/*----------------------------------------------------------------------------*/
/****************************************************************************/
/*---------------Returns ip adress (string form)------------------------------*/
void * get_in_addr(struct sockaddr *addr)
{
  if (addr->sa_family == IPV4_F)
    return &(((struct sockaddr_in*)addr)->sin_addr);
  return &(((struct sockaddr_in6*)addr)->sin6_addr);
}
/*----------------------------------------------------------------------------*/
/*--------------------Returns Port -------------------------------------------*/
unsigned int get_in_port(struct sockaddr *addr)
{
  if (addr->sa_family == IPV4_F)
    return ntohs(((struct sockaddr_in*)addr)->sin_port);
  return ntohs(((struct sockaddr_in6*)addr)->sin6_port);
}
/*----------------------------------------------------------------------------*/
/*-----------------------Returns Family---------------------------------------*/
unsigned int get_in_fam(struct sockaddr *addr)
{
  if (addr->sa_family == IPV4_F)
    return (((struct sockaddr_in*)addr)->sin_family);
  return (((struct sockaddr_in6*)addr)->sin6_family);
}
/*----------------------------------------------------------------------------*/
