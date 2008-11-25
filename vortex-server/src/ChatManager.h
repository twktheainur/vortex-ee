/*
 * ChatManager.h
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#ifndef CHATMANAGER_H_
#define CHATMANAGER_H_

#include "../../common/Thread.h"

class ChatManager: public Thread
{
public:
	ChatManager();
	virtual ~ChatManager();
};

#endif /* CHATMANAGER_H_ */
