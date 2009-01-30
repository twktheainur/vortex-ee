/*
 * ConnectionManagerOut.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "ConnectionManagerOut.h"
#include "../common/LoginResponsePacket.h"
#include "../common/Exception.h"
#include "../common/UpdatePacket.h"
#include "globals.h"


ConnectionManagerOut::ConnectionManagerOut(Client * cli)
{
    this->cli=cli;
    start((void *)this);
}
void ConnectionManagerOut::execute(void * arg)
{
    ConnectionManagerOut * pthis = (ConnectionManagerOut *)arg;
    flog<<"Connection manager Out Init...\n";
    TCPSocket * socket;
    event_data_t event_data;
    try
    {
        socket = pthis->cli->getClient()->getSocket();
    }
    catch (vortex::Exception * e)
    {
        printf("%s\n",e->what());
        flog <<"Cli init:"<<e->what()<<endl;
        delete e;
    }
    if ((event_data = connectionManagerOutEvent.getEvent()).type==event_connect_login)
    {
        //On expedie le paquet
        flog<<"got a login event here! hurry!!!!!\n";
        bitBuffer tmp;
        size_t hlen = 5;
        size_t length=event_data.data.length();
        tmp.writeChar(event_connect_login);
        tmp.writeInt(length);
        socket->send(tmp.set(),&hlen,0);
        socket->send(event_data.data.set(),&length,0);
        // Now on attends une reponse
        // On va d'abbord essayer de recuperer 5o (taille de l'entete global)
    }

    //LOGIN OK!
    //On peux rentrer dans la boucle principale de protocole
    while (1)
    {
        flog << "Main LOOP!\n";
        //Apres on check les events:
        if (connectionManagerOutEvent.changed())
        {
            flog << "Out Events?!!\n";
            event_data_t ev_data;
            ev_data = connectionManagerOutEvent.getEvent();
            switch (ev_data.type)
            {
            case event_connect_login:
                flog << "login" << endl;
                break;
            case event_connect_logout:
                flog << "logout" << endl;
                socket->free();
                pthis->cancel();
                flog << "Shutting Down!" << endl;
                break;
            case event_world_add:
                flog << "add" << endl;
                break;
            case event_world_update:
            {
                flog << "update" << endl;
                try
                {


                    UpdatePacket up(ev_data.data.readFloat(true),ev_data.data.readFloat(true),ev_data.data.readFloat(true),
                                    ev_data.data.readFloat(true),ev_data.data.readFloat(true),ev_data.data.readString(true));
                    size_t hlen = up.getHeaderSize();
                    size_t dlen= up.getDataSize();

                    socket->send(up.getHeader().set(),&hlen,0);
                    socket->send(up.getData().set(),&dlen,0);
                }
                catch (vortex::Exception * e)
                {
                    flog << "Waht 3"<<e->what() << endl;
                }
                break;
            }
            case event_world_del:
                flog << "del" << endl;
                break;
            case event_chat_incoming:
                flog << "chat" << endl;
                break;
            }
        }
        else
            Event::usleep(10);

    }
    flog << "main loop end\n";
}
ConnectionManagerOut::~ConnectionManagerOut()
{

}
