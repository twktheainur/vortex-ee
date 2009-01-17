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
	try{
			  pthis->application = new Application();
			  pthis->application->go();
    //while(1)
    #ifdef WIN32
      Sleep(1000);//milisecondes
    #else
//      usleep(1000000);//microsecondes
    #endif
			}

		catch(Ogre::Exception& e)
		{
			fprintf(stderr, "An exception has occurred: %s\n",
			e.getFullDescription().c_str());
		}
}
