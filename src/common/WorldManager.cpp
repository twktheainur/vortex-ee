/*
 * WorldManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "WorldManager.h"
extern Event eventManagerEvent;
extern Event worldManagerEvent;

WorldManager::WorldManager()
{
   event = &worldManagerEvent;

}

WorldManager::~WorldManager()
{
	// TODO Auto-generated destructor stub
}
