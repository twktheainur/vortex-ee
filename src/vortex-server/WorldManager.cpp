/*
 * WorldManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "WorldManager.h"
#include "World.h"
extern Event worldManagerEvent;

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
      if(worldManagerEvent.changed())
      {
        event_data_t data;
      }
      Event::usleep(10000);
    }

}


WorldManager::~WorldManager()
{
	// TODO Auto-generated destructor stub
}
