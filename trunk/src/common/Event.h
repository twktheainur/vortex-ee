/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/

#ifndef EVENT_H_
#define EVENT_H_
#if defined(__WIN32__) || defined(_WIN32)
#include <winsock2.h>
#else
extern "C"
{
#include <sys/select.h>
#include <pthread.h>
}
#endif
#include <string>
#include <queue>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "Mutex.h"
#include "Semaphore.h"
#include "bitBuffer.h"
using namespace std;

typedef struct world_event
{
  static const char update = 3;
  static const char add = 4;
  static const char del = 5;
} world_event_t;

typedef struct ogre_event
{
  static const char update = 8;
  static const char add = 9;
  static const char del = 10;
} ogre_event_t;

typedef struct chat_event
{
  static const char incoming = 6;
  static const char outgoing = 7;
} chat_event_t;

typedef struct connect_event
{
  static const char login = 0;
  static const char logout = 1;
} connect_event_t;

typedef struct event
{
  world_event_t world;
  chat_event_t chat;
  connect_event_t connect;
  ogre_event_t ogre;
} event_t;

typedef struct event_data
{
  int type;
  bitBuffer data;
} event_data_t;

class Event
{
private:
  Mutex mutex;
  //Cond cond;
  Semaphore semaphore;
  int listeners;
  queue<event_data_t> events;
  void wait();
public:
  Event();
  void sendEvent(int evt, bitBuffer &data);
  event_data_t getEvent();
  static event_t event;
  static event_t net_event;

  inline static void usleep(long delai)
  {
    printf("ST SLEEP\n");
       pthread_cond_t cw; /* condition "privée" utilisée par le thread qui veut dormir     */
     pthread_mutex_t mx; /* mutex nécessaire pour pouvoir utiliser pthread_cond_timedwait */
 
     // Ceci compile sous Linux mais pas Solaris
     struct timeval tps_deb; /* heure de debut du blocage                                     */
     struct timespec tps_exp; /* heure d'expiration du blocage                                 */
 
     pthread_cond_init(&cw, NULL);
     pthread_mutex_init(&mx, NULL);
 
     gettimeofday(&tps_deb, 0); /* on récupère l'heure courante             */
     tps_exp.tv_sec = tps_deb.tv_sec +delai/10000;
     tps_exp.tv_nsec = (tps_deb.tv_usec+delai-(delai/10000)*10000)*1000;/*+ delai*1000;*/ /* on convertit les ms en ns                */
     pthread_cond_timedwait(&cw, &mx, &tps_exp); /* et on se bloque jusqu'au temps calculé   */
 
     pthread_cond_destroy(&cw);
     pthread_mutex_destroy(&mx);
     printf("END SLEEP\n");
  }
};

#endif /* EVENT_H_ */

