/*
 * Semaphore.h
 *
 *  Created on: 2 déc. 2008
 *      Author: tchechma
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_
#ifdef WIN32
  #include "c:\pthread\include\pthread.h"
  #include "c:\pthread\include\semaphore.h"
#else
extern "C"
{
  #include <pthread.h>
  #include <semaphore.h>
}
#endif
#include "Exception.h"
enum SemaphoreException
{
  E_INIT_FAIL,
  E_POST_FAIL,
  E_WAIT_FAIL,
  E_DESTROY_FAIL
};
class ExSemaphore : vortex::Exception
{
public:
  ExSemaphore(int exception, std::string param="")
  :vortex::Exception(exception)
  {
    switch(exception)
    {
      case E_INIT_FAIL:setError("Sem init failed.");break;
      case E_WAIT_FAIL:setError("Sem wait failed.");break;
      case E_POST_FAIL:setError("Sem post failed.");break;
      case E_DESTROY_FAIL:setError("Sem destroy failed.");break;
    }
  }
};

class Semaphore
{
private:
	sem_t semaphore;
  int value;
public:
  inline void post()
  {
  	value++;
  	if(sem_post(&semaphore)==-1)
  		throw new ExSemaphore(E_POST_FAIL);
  }
  inline void wait()
  {
  	value--;
  	if(sem_wait(&semaphore)==-1)
  		throw new ExSemaphore(E_WAIT_FAIL);
  }
  inline int getValue(){return value;}
  Semaphore(int value);
  ~Semaphore();

};

#endif /* SEMAPHORE_H_ */
