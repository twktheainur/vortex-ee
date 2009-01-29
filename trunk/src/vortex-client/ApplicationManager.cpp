/*
 * ChatManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "ApplicationManager.h"
#include "../common/md5wrapper.h"
#include "../common/bitBuffer.h"
#include "globals.h"
#include "Client.h"
#include "ConnectionManager.h"

ApplicationManager::ApplicationManager(Client * cli)
{
  this->cli=cli;
  start((void*)this);
}
void ApplicationManager::execute(void * arg)
{
	ApplicationManager * pthis = (ApplicationManager *)arg;
	printf("Application manager Init...\n");

	md5wrapper md5;
	string user="twk";
	string pass="toto";
    pass = md5.getHashFromString(pass);
    bitBuffer buff;
    buff.writeString(user);
    buff.writeString(pass);
    connectionManagerOutEvent.sendEvent(event_connect_login,buff);

	try
	{
      pthis->application = new Application();
	    pthis->application->go();
	    bitBuffer buff2;
	    connectionManagerOutEvent.sendEvent(event_quit,buff2);
    }

		catch(Ogre::Exception& e)
		{
			fprintf(stderr, "An exception has occurred: %s\n",
			e.getFullDescription().c_str());
		}
}
