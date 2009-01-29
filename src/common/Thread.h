
#ifndef THREAD_H_
#define THREAD_H_

extern "C"
{
  #include <pthread.h>
}
#include <string>
#include "Exception.h"

enum ThreadExceptions
{
  E_CREATE_FAIL,
  E_JOIN_FAIL
};

class Thread
{
private:
	void * arg;
	pthread_t threadId;
protected:
	void run(void * arg);
	static void * entryPoint(void * arg);
	virtual void setup();
	virtual void execute(void * arg);
	inline void * getArg(){return arg;}
	inline void setArg(void * a){arg=a;}
public:
	Thread();
	void cancel();
	void join();
	virtual ~Thread();
	int start(void * arg);
};

class ExThread : vortex::Exception
{
public:
  ExThread(int exception, std::string param="")
  :vortex::Exception(exception)
  {
    switch(exception)
    {
      case E_CREATE_FAIL:setError("Unable to send data to destination");break;
    }
  }
};
#endif /* THREAD_H_ */
