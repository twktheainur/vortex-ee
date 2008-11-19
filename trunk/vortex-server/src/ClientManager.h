/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/

#ifndef CLIENTMANAGER_H_
#define CLIENTMANAGER_H_
extern "C"
{
  #include <pthread.h>
}
#include "Event.h"
#include "TCPSocket.h"
class ClientManager
{
private:
  pthread_t clientThread;
  Event event;

public:

  friend void * client_thread_handler(void * arg);
  friend void client_handler(TCPSocket socket);
  inline pthread_t getClientThread(){return clientThread;}
  inline Event getEvent(){return event;}
  ClientManager();
  virtual
  ~ClientManager();
};

#endif /* CLIENTMANAGER_H_ */
