#ifndef CLIENT_H_
#define CLIENT_H_
extern "C"
{
  #include <pthread.h>
}
#include "globals.h"
#include "TCPClient.h"
//#include "ChatManager.h"
#include "EventManager.h"
#include <vector>

class ConnectionManagerIn;
class ConnectionManagerOut;
class TCPClient;
class ApplicationManager;
class WorldManager;
class Client
{
private:
  //Thread Classes
  ConnectionManagerIn * connectionManagerInThread;
  ConnectionManagerOut * connectionManagerOutThread;
  WorldManager * worldManagerThread;
  //ChatManager * chatManagerThread;
  ApplicationManager * applicationManagerThread;
  //Event interface, has to point to a global scope variable
  //Clients will be registered in here by the TCPServer through the ConnectionManager
  TCPClient * client;
public:
	inline void setClient(TCPClient * cli){client=cli;}
	inline TCPClient * getClient(){return client;}
	inline ConnectionManagerIn * getConnectionManagerInThread()
	{
	  return connectionManagerInThread;
  }
  inline ConnectionManagerOut * getConnectionManagerOutThread()
	{
	  return connectionManagerOutThread;
  }
	//inline Event * getEventManagerEvent(){return eventManagerEvent;}
	//Here event has to point to a global scope variable sharable by threads
  Client();
  ~Client();
};
#endif
