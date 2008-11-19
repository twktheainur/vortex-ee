/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#include "Server.h"

Server::Server()
{
  int res = pthread_create(connection_manager_thread,NULL,connection_manager,NULL);

}

Server::~Server()
{
}

void * Server::connection_manager(void * arg)
{
  ClientManager mgr;
  server = new TCPServer(5,(string)"8080",mgr.client_handler);
  Server::clients.insert(mgr);
}
