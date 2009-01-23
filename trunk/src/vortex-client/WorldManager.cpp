/*
 * WorldManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "WorldManager.h"


WorldManager::WorldManager()
{
   event = &worldManagerEvent;
   start((void*)this);
}

void WorldManager::execute(void * arg)
{
	WorldManager * pthis = (WorldManager *)arg;
	pthis->world = new World();
	while(1)
	{
      if(ogreManagerEvent.changed())
      {
        event_data_t data;
      }
    }

}


WorldManager::~WorldManager()
{
	// TODO Auto-generated destructor stub
}
