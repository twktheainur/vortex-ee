/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the	 specific language governing rights and limitations
under the License.*/
#include "Server.h"
#include <cstdlib>
Server::Server()
{
  connectionManagerThread = new ConnectionManager(this);
  struct timeval tval;
  //int i = 1;
  tval.tv_usec=500000;
    while(getc(stdin)!='q')
    {
      //usleep(1000000);
      //cout << "DING! : "<< i << endl;
      //i++;
    }

}

Server::~Server()
{
	delete connectionManagerThread;
	delete server;
	std::vector<ClientManager*>::iterator it;
	for(it=clients.begin();it!=clients.end();it++)
		delete *it;
}
