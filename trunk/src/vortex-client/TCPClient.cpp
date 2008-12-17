/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#include "TCPClient.h"
#include "../common/BinBitSet.h"
TCPClient::TCPClient(string host,string service)
{
  struct addrinfo * p;
  socket = new TCPSocket(host,service,ANY_F);
  for(p = socket->getInfo();p!=NULL;p=p->ai_next)
  {
    try
    {
      socket->socket(p);
      socket->connect(p);
      break;
    }
    catch(ExTCPSocket * e)
    {
      printf("TCPClient:%s\n",e->what());
      puts("Une erreur a été rencontrée. Un serveur est-il actif ?");
      throw new ExTCPClient(E_CONNECT_ERROR,e->what());
      delete e;
    }
  }
  if(p==NULL)
    throw new ExTCPClient(E_SOCKET_ERROR);
  socket->setInfo(p);
}


TCPClient::~TCPClient()
{
  delete socket;
}

int TCPClient::send(const void * buffer, size_t * length, int flags)
{
  return socket->send(buffer,length,flags);
}
int TCPClient::recv(void * buffer, size_t length, int flags)
{
  return socket->recv(buffer,length,flags);
}

void TCPClient::recv(string & data)
{
	 try
	  {
	    size_t length = data.size();
	    socket->recv((char *)data.data(),length,0);
	  }
	  catch(ExSocket e)
	  {
	    throw;
	  }
}

void TCPClient::send(string & data)
{
	 try
	  {
	    size_t length = data.size();
	    socket->send(data.data(),&length,0);
	  }
	  catch(ExSocket e)
	  {
	    throw;
	  }
}

void TCPClient::protocolLoop()
{
	vector<unsigned char> toto;
	toto.reserve(3);
	string test;
	test.resize(150);
	test="The dice has been cast now, the world will be changed forever!";
	BinBitSet set;
  set.pushBit(0);
	set.pushBit(1);
	set.pushBit(0);
	set.pushBits<short>(345);

 while(1)
  {
    //try
    //{

  		//send(test);
  		usleep(1000000);
  		cout << test <<endl;
    //}
  	//catch(...)
  	//{}
  }


  	//long test1 = set.popBitsL(3);
  	//short test = set.popBits<short>();
  	//cout << test1 << endl << test << endl;
}
