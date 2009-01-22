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
#include "../common/LoginResponsePacket.h"

ClientManager::ClientManager(TCPSocket sock)
: Thread()
{
	socket = sock;
	start((void*) (&sock));
}

ClientManager::~ClientManager()
{
}

void ClientManager::execute(void * arg)
{
	TCPSocket local_socket = *((TCPSocket*) arg);
	printf("New Client!\n");
	bitBuffer buff;
	char * headbuff = (char *)malloc(5);
	char * databuff;
	try
	{
			local_socket.recv(headbuff,5, 0);
			LoginPacket lp = LoginPacket(headbuff,5);
            databuff = (char *)malloc(lp.getDataSize());
            local_socket.recv(databuff,lp.getDataSize(),0);
            lp.setData(databuff,lp.getDataSize());
            //Verif dans la base de donnees si le login est accepte
            if(true)
            {
              LoginResponsePacket lpr("Hell");
              size_t hlen = lpr.getHeaderSize();
              size_t dlen = lpr.getDataSize();
              local_socket.send(lpr.getHeader().set(),&hlen,0);
              local_socket.send(lpr.getData().set(),&dlen,0);
            }
		while (1)
		{
          //Polling to check whether there is data to read...
          if(true/*replace by select result test*/)
          {
            headbuff = (char *)malloc(5);
            local_socket.recv(headbuff,5,0);
            bitBuffer header(headbuff,5);
            switch((int)header.readChar(true))
            {
                case event_connect_login:break;
                case event_connect_logout:break;
                case event_world_add:break;
                case event_world_update:break;
                case event_world_del:break;
                case event_chat_incoming:break;
                case event_chat_outgoing:break;

            }
          }
          if(event.changed())
          {
              event_data_t data;
              data = event.getEvent();
              switch(data.type)
              {
                case event_connect_login:break;
                case event_connect_logout:break;
                case event_world_add:break;
                case event_world_update:break;
                case event_world_del:break;
                case event_chat_incoming:break;
                case event_chat_outgoing:break;
              }

          }
		}
	}
	catch (ExSocket * e)
	{
		printf("Client Thread(recv):%s\n", e->what());
		delete e;
	}
	local_socket.free();
}
