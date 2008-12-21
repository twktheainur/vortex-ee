/*
 * ChatManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "ApplicationManager.h"

ApplicationManager::ApplicationManager(Event * event)
{
  main_event = event;
  start((void*)this);
}
void ApplicationManager::execute(void * arg)
{
	ApplicationManager * pthis = (ApplicationManager *)arg;
	try{
			  pthis->application = new Application();
			  pthis->application->go();
    while(1)
      usleep(1000000);
			}

		catch(Ogre::Exception& e)
		{
			fprintf(stderr, "An exception has occurred: %s\n",
			e.getFullDescription().c_str());
		}
}
