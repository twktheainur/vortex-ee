/*
 * Semaphore.cpp
 *
 *  Created on: 2 déc. 2008
 *      Author: tchechma
 */

#include "Semaphore.h"

Semaphore::Semaphore(int value)
{
  if(sem_init(&semaphore,0,value)==-1)
    throw new ExSemaphore(E_INIT_FAIL);
}

Semaphore::~Semaphore()
{
  if(sem_destroy(&semaphore)==-1)
  	throw new ExSemaphore(E_DESTROY_FAIL);
}
