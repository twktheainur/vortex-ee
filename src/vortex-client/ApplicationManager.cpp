/*
 * ChatManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "ApplicationManager.h"

ApplicationManager::ApplicationManager()
{
  start((void*)this);
}
void ApplicationManager::execute(void * arg)
{
	ApplicationManager * pthis = (ApplicationManager *)arg;
	printf("Application manager Init...\n");
	try{
			  pthis->application = new Application();
			  pthis->application->go();
			}

		catch(Ogre::Exception& e)
		{
			fprintf(stderr, "An exception has occurred: %s\n",
			e.getFullDescription().c_str());
		}
}
