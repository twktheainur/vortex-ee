/*
 * EventManager.h
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include "../../common/Thread.h"

class EventManager: public Thread
{
public:
	EventManager();
	virtual ~EventManager();
};

#endif /* EVENTMANAGER_H_ */
