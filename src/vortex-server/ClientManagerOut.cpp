/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/

#include "ClientManagerOut.h"
#include "../common/bitBuffer.h"
#include "globals.h"
#include "../common/Exception.h"
#include "../common/LoginPacket.h"
#include "../common/LoginResponsePacket.h"
#include "../common/UpdatePacket.h"

ClientManagerOut::ClientManagerOut(TCPSocket sock)
        : Thread()
{
    socket = sock;
    start((void*) (&sock));
}

ClientManagerOut::~ClientManagerOut()
{
}

void ClientManagerOut::execute(void * arg)
{
    TCPSocket local_socket = *((TCPSocket*) arg);
    printf("New Client!\n");
    bitBuffer buff;
    char * headbuff = NULL;
    headbuff=(char *)malloc(5);
    char * databuff=NULL;
    bool ok = false;
    int iter=0;
    try
    {
        event.getEvent();
        cout << "Got it!";
        ok = true;
        while (ok)
        {
            //Polling to check whether there is data to read...
            cout << "Out loop\n";
                event_data_t data;
                data = event.getEvent();
                switch (data.type)
                {
                case event_connect_login:
                    break;
                case event_connect_logout:
                    break;
                case event_world_add:
                    break;
                case event_world_update:
                    break;
                case event_world_del:
                    break;
                case event_chat_incoming:
                    break;
                }
        }
    }
    catch (vortex::Exception * e)
    {
        printf("Client Thread:%s\n", e->what());
        delete e;
    }
    local_socket.free();
}
