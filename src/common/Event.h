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

typedef enum event
{
  event_world_update = 3,
  event_world_add = 4,
  event_world_del = 5,
  event_ogre_update = 8,
  event_ogre_add = 9,
  event_ogre_del = 10,
  event_chat_incoming = 6,
  event_connect_login = 0,
  event_connect_logout = 1,
  event_anim_avant = 11,
  event_anim_arriere = 12,
  event_anim_gauche = 13,
  event_anim_droite = 14,
  event_anim_aucune=15,
  event_quit=16
}event_t;


typedef struct event_datatype
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
  inline bool changed()
  {
    mutex.lock();
    bool test = (events.size()!=0);
    mutex.unlock();
    return test;
  }

  inline static void usleep(long delai)
  {
    #ifdef WIN32
      Sleep(delai/10);//le +1 sert a éviter Sleep(0), qui ne garantit pas que le thread ne sera pas actif
    #else
      usleep(delai);
    #endif
  }
};

#endif /* EVENT_H_ */

