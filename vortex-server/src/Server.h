/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#ifndef SERVER_H_
#define SERVER_H_
extern "C"
{
  #include <pthread.h>
}
#include "ClientManager.h"
#include "TCPServer.h"
#include <set>
class Server
{
private:
  pthread_t * connection_manager_thread;
  pthread_t * world_manager_thread;
  pthread_t * chat_manager_thread;
  pthread_t * event_manager_thread;
  static set<ClientManager> clients;
  static TCPServer * server;
public:
  Server();
  static void * connection_manager(void * arg);
  static void * world_manager(void * arg);
  static void * chat_manager(void * arg);
  static void * event_manager(void * arg);
  virtual ~Server();
};

#endif /*SERVER_H_*/
