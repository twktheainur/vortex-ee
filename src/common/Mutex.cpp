/*
 * Mutex.cpp
 *
 *  Created on: Nov 25, 2008
 *      Author: twk
 */

#include "Mutex.h"
#include <cerrno>

Mutex::Mutex()
{
  if(pthread_mutex_init(&mutex,NULL)!=0)
  {
  	printf("Mutex.cpp -- 15: init");
  	throw new ExMutex(E_MUTEXINIT_FAIL);
  }
}

Mutex::~Mutex()
{
	 if(pthread_mutex_destroy(&mutex)!=0)
	 {
		  printf("Mutex.cpp -- 15: destr");
	  	throw new ExMutex(E_MUTEXDESTROY_FAIL);
	 }
}

void Mutex::lock()
{
  if(pthread_mutex_lock(&mutex)!=0)
  {
  	printf("Mutex.cpp -- 15: lock");
  	throw new ExMutex(E_MUTEXLOCK_FAIL);
  }
}
void Mutex::unlock()
{
  if(pthread_mutex_unlock(&mutex)!=0)
  {
  	printf("Mutex.cpp -- 15: unlock");
  	throw new ExMutex(E_MUTEXUNLOCK_FAIL);
  }
}
bool Mutex::trylock()
{
	int lockval;
  if((lockval=pthread_mutex_trylock(&mutex))==0)
  	return true;
  else if(lockval==EBUSY)
  	return false;
  else
  {
  	printf("Mutex.cpp -- 15: trylock");
  	throw new ExMutex(E_MUTEXLOCK_FAIL);
  }
}
