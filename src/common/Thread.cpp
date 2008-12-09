/*
 * Thread.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "Thread.h"
#include <iostream>
extern "C"
{
  #include <pthread.h>
}
Thread::Thread() {}
Thread::~Thread()
{
}
void Thread::cancel()
{
  pthread_cancel(threadId);
}
int Thread::start(void * arg)
{
	setArg(arg);
	int ret = pthread_create(&threadId,NULL,Thread::entryPoint,this);
	std::cout << "Thread Spawned!"<<std::endl;
  return ret;
}

void Thread::run(void * arg)
{
  setup();
  execute(arg);
}
void * Thread::entryPoint(void * this_p)
{
	Thread * th_ptr = (Thread *) this_p;
	th_ptr->run(th_ptr->getArg());
	return (void *)NULL;
}
void Thread::setup()
{}// TODO Intialisation operations (to be overloaded)
void Thread::execute(void * arg)
{}//TODO : Code to be executed (to be overloaded in derived classes)
