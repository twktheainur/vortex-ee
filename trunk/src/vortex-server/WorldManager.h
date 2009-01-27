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
#include "World.h"
#include "globals.h"

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
