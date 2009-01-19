/*
 * ConnectionManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "ConnectionManager.h"
#include "../common/LoginResponsePacket.h"
#include "../common/Exception.h"


ConnectionManager::ConnectionManager(Client * cli)
{
	this->cli=cli;
	start((void *)this);
}
void ConnectionManager::execute(void * arg)
{
	ConnectionManager * pthis = (ConnectionManager *)arg;
    TCPSocket * socket;
    event_data_t event_data;
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
    if((event_data = connectionManagerEvent.getEvent()).type==Event::event.connect.login)
      {
        //On expedie le paquet
        size_t length =event_data.data.length();
        socket->send(event_data.data.set(),&length,0);
        // Now on attends une reponse
        // On va d'abbord essayer de recuperer 4o (taille de l'entete global)

        char * buffer = (char *)malloc(4);
        socket->recv(buffer,4,0);
        try
        {
          LoginResponsePacket lrp(buffer,4);//Classe pas encore ecrite mais ca ne saurais tarder
        }
        catch(Exception * e)
        {
            cout;
          //Soit Mauvais paquet
          //Soit timeout du socket
        }
        free(buffer);
      }

      //LOGIN OK!
      //On peux rentrer dans la boucle principale de protocole
 while(1)
  {
    try
    {
      //D'abord on checks si il y a des donnees a lire avec select

      //Apres on check les events:
      if(connectionManagerEvent.changed())
      {
        event_data_t ev_data;
        ev_data = connectionManagerEvent.getEvent()
      }
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
