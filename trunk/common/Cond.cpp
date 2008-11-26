

#include "Cond.h"
Cond::Cond()
{
	if(!pthread_cond_init(&cond,NULL))
		throw new ExCond(E_INIT_FAIL);
}

Cond::~Cond()
{
	if(!pthread_cond_destroy(&cond))
		throw new ExCond(E_DESTROY_FAIL);
}

void Cond::wait(Mutex * mut)
{
  if(!pthread_cond_wait(&cond,&(mut->getMutex())))
  	throw new ExCond(E_WAIT_FAIL);
}
void Cond::signal()
{
  if(!pthread_cond_signal(&cond))
  	throw new ExCond(E_SIGNAL_FAIL);
}
void Cond::broadcast()
{
  if(!pthread_cond_broadcast(&cond))
  	throw new ExCond(E_SIGNAL_FAIL);
}
