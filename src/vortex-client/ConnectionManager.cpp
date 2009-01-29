/*
 * ConnectionManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "ConnectionManager.h"
#include "../common/LoginResponsePacket.h"
#include "../common/Exception.h"
#include "../common/UpdatePacket.h"


ConnectionManager::ConnectionManager(Client * cli)
{
    this->cli=cli;
    start((void *)this);
}
void ConnectionManager::execute(void * arg)
{
    ConnectionManager * pthis = (ConnectionManager *)arg;
    flog<<"Connection manager Init...\n";
    TCPSocket * socket;
    event_data_t event_data;
    try
    {
        pthis->cli->setClient(new TCPClient("localhost","8080"));
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

        char * buffer = (char *)malloc(5);
        try
        {
        if (socket->pollRead())
            socket->recv(buffer,5,0);
            LoginResponsePacket lrp(buffer,(size_t)5);
            if (lrp.getHeader().length()>0)
                flog << "Yay got a positive response!\n";
            else
                flog << "login failed!\n";
        }
        catch (vortex::Exception * e)
        {

            flog << "what 1"<<e->what()<<endl;
            //Soit Mauvais paquet
            //Soit timeout du socket
        }
        free(buffer);
    }

    //LOGIN OK!
    //On peux rentrer dans la boucle principale de protocole
    while (1)
    {
        flog << "Main LOOP!\n";
        try
        {
            //D'abord on checks si il y a des donnees a lire avec select
            flog << "POLL successful!\n";
            char * buffer = (char *)malloc(5);
            socket->recv(buffer,5,0);
            bitBuffer header(buffer,5);
            flog << "Type:" <<header.readChar(false)<<endl;

            switch (header.readChar(false))
            {

            case event_connect_login:
                flog << "login" << endl;
                break;
            case event_connect_logout:
                flog << "logout" << endl;
                break;
            case event_world_add:
                flog << "add" << endl;
                break;
            case event_world_update:
                flog << "update" << endl;
                break;
            case event_world_del:
                flog << "del" << endl;
                break;
            case event_chat_incoming:
                flog << "chat" << endl;
                break;

            }
            free(buffer);
        }
        catch (vortex::Exception * e)
        {
            flog << "what2"<<e->what()<<endl;
            delete e;
        }
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
ConnectionManager::~ConnectionManager()
{

}
