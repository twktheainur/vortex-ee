

#include "Cond.h"
Cond::Cond()
{
	if(pthread_cond_init(&cond,NULL))
		throw new ExCond(E_INIT_FAIL);
}

Cond::~Cond()
{
	if(!pthread_cond_destroy(&cond))
		throw new ExCond(E_DESTROY_FAIL);
}

void Cond::wait(bool condition)
{
	pthread_mutex_t mut = mutex.getMutex();
  if(condition)
	  mutex.lock();
	printf("STWAIT\n");
  if(pthread_cond_wait(&cond,&(mut)))
    throw new ExCond(E_WAIT_FAIL);
	mutex.unlock();
}
void Cond::signal()
{
  if(pthread_cond_signal(&cond))
  	throw new ExCond(E_SIGNAL_FAIL);
}
void Cond::broadcast()
{
  if(pthread_cond_broadcast(&cond))
  	throw new ExCond(E_SIGNAL_FAIL);
}
