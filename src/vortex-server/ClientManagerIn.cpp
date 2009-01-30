/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/

#include "ClientManagerOut.h"
#include "ClientManagerIn.h"
#include "../common/bitBuffer.h"
#include "globals.h"
#include "../common/Exception.h"
#include "../common/LoginPacket.h"
#include "../common/LoginResponsePacket.h"
#include "../common/UpdatePacket.h"

ClientManagerIn::ClientManagerIn(TCPSocket sock,ClientManagerOut * cmo)
        : Thread()
{
    socket = sock;
    this->cmo=cmo;
    start((void*) (&sock));
}

ClientManagerIn::~ClientManagerIn()
{
}

void ClientManagerIn::execute(void * arg)
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
        local_socket.recv(headbuff,5, 0);
        LoginPacket lp = LoginPacket(headbuff,5);
        databuff = (char *)malloc(lp.getDataSize());
        local_socket.recv(databuff,lp.getDataSize(),0);
        lp.setData(databuff,lp.getDataSize());
        //Verif dans la base de donnees si le login est accepte
        if (lp.getLogin()=="twk")
        {
            cout << lp.getLogin()<<endl;
            cout << lp.getPassHash()<<endl;
            cout << "Building Response\n";
            LoginResponsePacket lpr(lp.getLogin());
            size_t hlen = lpr.getHeaderSize();
            size_t dlen = lpr.getDataSize();
            local_socket.send(lpr.getHeader().set(),&hlen,0);
            local_socket.send(lpr.getData().set(),&dlen,0);
            cout << "Stuff Sent!\n";
            cmo->getEvent().sendEvent(event_connect_login,lp.getData());
        }
        else
        {
          cout << "wrong pass biatch!\n";
          cmo->cancel();
          this->cancel();
        }
        ok = true;
        while (ok)
        {
            //Polling to check whether there is data to read...
            cout << "Server ClientManagerIn Main Protocol Loop!"<<endl;
                headbuff = (char *)malloc(5);
                try
                {
                    local_socket.recv(headbuff,5,0);
                    cout << "Poll on read positive!" << endl;
                }
                catch (vortex::Exception * e)
                {
                    cout<<"EX nothing to recieve!"<<endl;;
                    Event::usleep(1000);
                }
                bitBuffer header(headbuff,5);
                switch ((int)header.readChar(true))
                {
                case event_connect_login:
                    cout << "login" << endl;
                    lp = LoginPacket(header.set(),5);
                    cout << "Size" <<lp.getDataSize() << endl;
                    databuff = (char *)malloc(lp.getDataSize());
                    try
                    {
                        local_socket.recv(databuff,lp.getDataSize(),0);
                    }
                    catch (vortex::Exception * e)
                    {
                        cout<<"recv_failed: The client has quit!Damn the bastard!"<<endl;;
                        break;
                    }
                    lp.setData(databuff,lp.getDataSize());

                    break;
                case event_connect_logout:
                    cout << "logout" << endl;

                    break;
                case event_world_add:
                    cout << "add" << endl;


                    break;
                case event_world_update:
                {
                    cout << "update" << endl;
                    UpdatePacket up(header.set(),header.length());
                    cout << "Size" <<up.getDataSize() << endl;
                    databuff = (char *)malloc(up.getDataSize());
                    try
                    {
                        local_socket.recv(databuff,up.getDataSize(),0);
                    }
                    catch (vortex::Exception * e)
                    {
                        cout<<"recv_failed: The client has quit!Damn the bastard!"<<endl;;
                        break;
                    }
                    up.setData(databuff,lp.getDataSize());
                    worldManagerEvent.sendEvent(event_world_update,up.getData());
                    break;
                }
                case event_world_del:
                    cout << "del" << endl;
                    break;
                case event_chat_incoming:
                    cout << "chat" << endl;
                    break;
                    //something's wrong!
                default:
                    ok=false;
                    break;
                }
                free(headbuff);
                free(databuff);
            printf("Polled\n");
            if (event.changed())
            {
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
    }
    catch (vortex::Exception * e)
    {
        printf("Client Thread:%s\n", e->what());
        delete e;
    }
    local_socket.free();
}
