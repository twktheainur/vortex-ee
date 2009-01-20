/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/

#include "ClientManager.h"
#include "../common/bitBuffer.h"
#include "globals.h"
#include "../common/LoginPacket.h"

ClientManager::ClientManager(TCPSocket sock)
: Thread()
{
	//this->event = &clientManagerEvent;
	socket = sock;
	start((void*) (&sock));
}

ClientManager::~ClientManager()
{
}

void ClientManager::execute(void * arg)
{
	TCPSocket local_socket = *((TCPSocket*) arg);
	bitBuffer buff;
	char * headbuff = (char *)malloc(5);
	char * databuff;
	try
	{

			local_socket.recv(headbuff,5, 0);
			LoginPacket lp = LoginPacket(headbuff,5);
            databuff = (char *)malloc(lp.getDataSize());
            local_socket.recv(databuff,lp.getDataSize(),0);
            //lp.setData(databuff,lp.getDataSize());
			worldManagerEvent.sendEvent(Event::event.connect.login, buff);
		while (1)
		{
          Sleep(100);

		}
	}
	catch (ExSocket * e)
	{
		printf("Client Thread(recv):%s\n", e->what());
		delete e;
	}
	local_socket.free();
}
