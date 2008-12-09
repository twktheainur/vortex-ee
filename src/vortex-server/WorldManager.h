/*
 * WorldManager.h
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#ifndef WORLDMANAGER_H_
#define WORLDMANAGER_H_

#include "../../common/Thread.h"

class WorldManager: public Thread
{
public:
	WorldManager();
	virtual ~WorldManager();
};

#endif /* WORLDMANAGER_H_ */
