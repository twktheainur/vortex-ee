/*
 * Mutex.h
 *
 *  Created on: Nov 25, 2008
 *      Author: twk
 */

#ifndef MUTEX_H_
#define MUTEX_H_
extern "C"
{
#include <pthread.h>
}
#include "Exception.h"
enum MutexExceptions
{
  E_MUTEXINIT_FAIL,
  E_MUTEXLOCK_FAIL,
  E_MUTEXUNLOCK_FAIL,
  E_MUTEXDESTROY_FAIL
};
class Mutex
{
private:
  pthread_mutex_t mutex;
public:
	void lock();
	void unlock();
	bool trylock();
	inline pthread_mutex_t getMutex(){return mutex;}
	Mutex();
	~Mutex();
};

class ExMutex : vortex::Exception
{
public:
  ExMutex(int exception, std::string param="")
  :vortex::Exception(exception)
  {
    switch(exception)
    {
      case E_MUTEXINIT_FAIL:setError("Mutex init failed.");break;
      case E_MUTEXLOCK_FAIL:setError("Mutex lock failed.");break;
      case E_MUTEXUNLOCK_FAIL:setError("Mutex unlock failed.");break;
      case E_MUTEXDESTROY_FAIL:setError("Mutex destroy failed.");break;
    }
  }
};

#endif /* MUTEX_H_ */
