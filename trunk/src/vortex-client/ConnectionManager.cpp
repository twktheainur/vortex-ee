#include "../common/bitBuffer.h"
/*
 * ConnectionManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "ConnectionManager.h"
#if defined(WIN32)
void usleep(time_t usec)
{
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = usec;
  select(0, NULL, NULL, NULL, &timeout);
}
#endif

ConnectionManager::ConnectionManager(Client * cli)
{
	this->cli=cli;
	start((void *)this);
}
void ConnectionManager::execute(void * arg)
{
	ConnectionManager * pthis = (ConnectionManager *)arg;
  TCPSocket * socket;
	try
	{
 	  pthis->cli->setClient(new TCPClient("localhost","8080"));
    socket = pthis->cli->getClient()->getSocket();
  }
 	catch(Exception * e)
	{
		printf("%s\n",e->what());
		delete e;
	}

    bitBuffer buff;
    buff.write<char>(Event::event.connect.login,true);
    buff.write<std::string>("Hello there miserable suckers!",true);
    size_t size = buff.length();

 while(1)
  {
    try
    {
  		socket->send(buff.set(),&size,0);

  		#ifdef WIN32
  		//_sleep(5000); //millisecondes. Ca , n'a pas l'air de marcher
  		#else
  		usleep(5000000);//microsecondes
  		#endif
  		usleep(5000000);//microsecondes
  		cout << buff.length() <<endl;
    }
  	catch(Exception * e)
  	{
      cout << e->what() << endl;
      delete e;
    }
  }
}

ConnectionManager::~ConnectionManager()
{

}
