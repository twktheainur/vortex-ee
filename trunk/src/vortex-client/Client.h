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

class ConnectionManager;
class TCPClient;
class ApplicationManager;
class WorldManager;
class Client
{
private:
  //Thread Classes
  ConnectionManager * connectionManagerThread;
  WorldManager * worldManagerThread;
  //ChatManager * chatManagerThread;
  ApplicationManager * applicationManagerThread;
  //Event interface, has to point to a global scope variable
  //Clients will be registered in here by the TCPServer through the ConnectionManager
  TCPClient * client;
public:
	inline void setClient(TCPClient * cli){client=cli;}
	inline TCPClient * getClient(){return client;}
	inline ConnectionManager * getConnectionManagerThread()
	{
	  return connectionManagerThread;
  }
	//inline Event * getEventManagerEvent(){return eventManagerEvent;}
	//Here event has to point to a global scope variable sharable by threads
  Client();
  ~Client();
};
#endif
