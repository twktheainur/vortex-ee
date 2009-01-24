/*
 * WorldManager.cpp
 *
 *  Created on: Nov 22, 2008
 *      Author: twk
 */

#include "WorldManager.h"


WorldManager::WorldManager()
{
   event = &worldManagerEvent;
   start((void*)this);
}

void WorldManager::execute(void * arg)
{
	WorldManager * pthis = (WorldManager *)arg;
	pthis->world = new World();
	while(1)
	{

          bool update = false;

          event_data_t localEvent;
          world_user_t worldUser;

          // on gere d'abord les mises à jour locales
          while(ogreManagerEvent.changed())
          {
            //on check eventReceived.type et suivant le cas, on insere une nouvelle node au vecteur ou on en met une a jour
            localEvent = ogreManagerEvent.getEvent();
            switch (localEvent.type)
            {
                case 8: //update
                    update = right;
                    worldUser.px = localEvent.data.readFloat(true);
                    worldUser.py = localEvent.data.readFloat(true);
                    worldUser.pz = localEvent.data.readFloat(true);
                    worldUser.dx = localEvent.data.readFloat(true);
                    worldUser.dy = localEvent.data.readFloat(true);
                    worldUser.id = localEvent.data.readString(true);

                    pthis->world->update_user(worldUser);
                    // en cas d'update, on attend d'avoir pris toutes les maj en compte pour envoyer l'event au serveur
                break;

                case 9: //add
                    pthis->world->add_user(localEvent.data.readString(true));
                    connectionManagerEvent.sendEvent(9,localEvent.data); // si c'est une connexion on envoie directement l'event au serveur
                break;

                case 10: //del
                    connectionManagerEvent.sendEvent(10,localEvent.data); // si c'est une deconnexion on envoie directement l'event au serveur
                    pthis->world->delete_user(localEvent.data.readString(true));
                break;

                default:
                    break;
            }
          }

          if (update) // si on a eu un update on peut renvoyer le dernier en date au serveur
            connectionManagerEvent.sendEvent(localEvent.type,localEvent.data);
            // on note qu'ici, un event d'update peut etre envoye meme si l'user s'est deconnecte, mais le serveur l'ignorera

          update=false;


          event_data_t serverEvent;
          // puis on gere les mise a jour recues du serveur
          while(connectionManagerEvent.changed())
          {
            //on check eventReceived.type et suivant le cas, on insere une nouvelle node au vecteur ou on en met une a jour
            serverEvent = connectionManagerEvent.getEvent();
            switch (localEvent.type)
            {
                case 3: //update
                    worldUser.px = serverEvent.data.readFloat(true);
                    worldUser.py = serverEvent.data.readFloat(true);
                    worldUser.pz = serverEvent.data.readFloat(true);
                    worldUser.dx = serverEvent.data.readFloat(true);
                    worldUser.dy = serverEvent.data.readFloat(true);
                    worldUser.id = serverEvent.data.readString(true);

                    pthis->world->update_user(worldUser);

                    worldManagerEvent.sendEvent(3,serverEvent.data);
                break;

                case 4: //add
                    pthis->world->add_user(serverEvent.data.readString(true));
                    worldManagerEvent.sendEvent(4,serverEvent.data);
                break;

                case 5: //del
                    pthis->world->delete_user(serverEvent.data.readString(true));
                    worldManagerEvent.sendEvent(5,serverEvent.data);
                break;

                default:
                    break;
            }
          }

        // AJOUTER LE DELAI

    }
}


WorldManager::~WorldManager()
{
	// TODO Auto-generated destructor stub
}
