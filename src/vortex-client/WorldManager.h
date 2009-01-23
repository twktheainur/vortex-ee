/*
 * WorldManager.h
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#ifndef WORLDMANAGER_H_
#define WORLDMANAGER_H_

#include "../common/Thread.h"
#include "../common/Event.h"
#include "globals.h"
#include "World.h"
extern Event eventManagerEvent;
extern Event worldManagerEvent;

class WorldManager : public Thread
{
public:
  WorldManager();
  virtual ~WorldManager();
  void execute(void * arg);
private:
  Event * event;
  World * world;
};

#endif /* WORLDMANAGER_H_ */
