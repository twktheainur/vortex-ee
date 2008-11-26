
#ifndef COND_H_
#define COND_H_
extern "C"
{
#include <pthread.h>
}
#include "Exception.h"
#include "Mutex.h"
typedef enum CondExceptions
{
  E_WAIT_FAIL=100,
  E_INIT_FAIL,
  E_DESTROY_FAIL,
  E_SIGNAL_FAIL
}cond_exceptions_t;
class Cond
{
private:
  pthread_cond_t cond;
public:
	void wait(Mutex*);
	void signal();
	void broadcast();
	Cond();
	~Cond();
};

class ExCond : Exception
{
public:
  ExCond(int exception, std::string param="")
  :Exception(exception)
  {
    switch(exception)
    {
      case E_INIT_FAIL:setError("Cond init failed.");break;
      case E_WAIT_FAIL:setError("Cond wait failed.");break;
      case E_SIGNAL_FAIL:setError("Cond signal failed.");break;
      case E_DESTROY_FAIL:setError("Cond destroy failed.");break;
    }
  }
};


#endif /* COND_H_ */
