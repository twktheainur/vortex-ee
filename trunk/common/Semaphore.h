/*
 * Semaphore.h
 *
 *  Created on: 2 déc. 2008
 *      Author: tchechma
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_
extern "C"
{
  #include <pthread.h>
}

#include <pthread.h>

class Semaphore
{
private:
  int value;
public:
  inline void post(){value++;sem_post()}
  inline void wait(){}
  Semaphore();
  ~Semaphore();

};

#endif /* SEMAPHORE_H_ */
