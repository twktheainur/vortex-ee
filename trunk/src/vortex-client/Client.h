#ifndef CLIENT_H_
#define CLIENT_H_
extern "C"
{
  #include <pthread.h>
}
#include "TCPClient.h"
#include "ConnectionManager.h"
#include "ApplicationManager.h"
#include "../common/WorldManager.h"
//#include "ChatManager.h"
#include "EventManager.h"
#include "../common/Event.h"
#include <vector>

class ConnectionManager;
class TCPClient;
class Client
{
private:
	//Thread Classes
  ConnectionManager * connectionManagerThread;
  WorldManager * worldManagerThread;
  //ChatManager * chatManagerThread;
	EventManager * eventManagerThread;
  ApplicationManager * applicationManagerThread;
	//Event interface, has to point to a global scope variable
	Event * eventManagerEvent;
  //Clients will be registered in here by the TCPServer through the ConnectionManager
  TCPClient * client;
public:
	inline void setClient(TCPClient * cli){client=cli;}
	inline TCPClient * getClient(){return client;}
	inline Event * getEventManagerEvent(){return eventManagerEvent;}
	//Here event has to point to a global scope variable sharable by threads
  Client(Event * event);
  ~Client();
};
#endif
