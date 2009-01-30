/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#ifndef SOCKET_H
#define SOCKET_H

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
#define _WIN32_WINNT 0x501//pour indiquer qu'on a windows >= XP
#include <ws2tcpip.h>
#include <winsock2.h>
typedef SOCKET sock_t;
#define SOCK_ERR SOCKET_ERROR
#else
extern "C"
{
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <netdb.h>
#include <arpa/inet.h>

}
  typedef int sock_t;
#define SOCK_ERR -1
#endif
extern "C"
{
#include <errno.h>
}

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "Exception.h"

// IP Adress family
#define ANY_F AF_UNSPEC
#define IPV4_F AF_INET
#define IPV6_F AF_INET6

//Socket Type
#define UDP_SOCK  SOCK_DGRAM
#define TCP_SOCK  SOCK_STREAM

//Exceptions
enum SocketException
{
    E_GETADDRINFO_FAIL,
    E_SOCKET_FAIL,
    E_CLOSE_FAIL,
    E_BIND_FAIL,
    E_SEND_FAIL,
    E_POLL_FAIL,
    E_RECV_FAIL,
    E_SETSOCKOPT_FAIL
};

//General purpose function
void * get_in_addr(struct sockaddr *addr);
unsigned int get_in_port(struct sockaddr *addr);
unsigned int get_in_fam(struct sockaddr *addr);

//The Socket Class
class Socket
{
public:
    Socket(string host,string service,int family,int type);
    virtual ~Socket();
    virtual int send(const void * buffer, size_t * length, int flags);
    virtual int recv(void * buffer, size_t length, int flags);
    bool pollRead();
    int bind(struct addrinfo * addr);
    void socket(struct addrinfo * addr);
    void setSockOpt(int level, int optname, void * optval, size_t size);
    inline void setNblock()
    {
#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
        unsigned long enable = 1;
        ioctlsocket (sockFD,FIONBIO,&enable);
#else
        //::fcntl(sockFD, F_SETFL, O_NONBLOCK);
#endif
    }

    sock_t getSockFD();
    inline void setSockFD(sock_t new_fd)
    {
        sockFD=new_fd;
    }
    void setHost(string new_var);
    string getHost();
    void setService(string new_var);
    string getService() ;
    void setInfo(struct addrinfo *new_var)
    {
        info=new_var;
    }
    struct addrinfo * getInfo()
    {
        return info;
    }
    void free();
    friend Socket& operator<<(Socket&,std::basic_string<char, std::char_traits<char>, std::allocator<char> > data);
    friend Socket& operator>>(Socket&,std::basic_string<char, std::char_traits<char>, std::allocator<char> > &data);
protected:
    void initInfo(string host, string service, int family, int type);
private:
    sock_t sockFD;
    string host;
    string service;
    struct addrinfo * info;
};

class ExSocket : public vortex::Exception
{
public:
    ExSocket(int exception,string param="")
            :vortex::Exception(exception)
    {
        switch (exception)
        {
        case E_GETADDRINFO_FAIL:
            setError("Unable de retrieve address information:"+param);
            break;
        case E_SOCKET_FAIL:
            setError("Cannot open socket");
            break;
        case E_CLOSE_FAIL:
            setError("Cannot close socket");
            break;
        case E_BIND_FAIL:
            setError("Cannot bind port:"+param+"to socket");
            break;
        case E_SEND_FAIL:
            setError("An error occured while data was being sent:"+param);
            break;
        case E_RECV_FAIL:
            setError("An error occured while data was being received:"+param);
            break;
        }
    }
};

#endif // SOCKET_H
