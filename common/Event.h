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
#include "Mutex.h"
#include "Cond.h"
using namespace std;
typedef enum
{
  EV_NONE,
  EV_UPD,
  EV_ADD,
  EV_DEL,
  EV_INC_CHAT,
  EV_OUT_CHAT,
  EV_OTHER
}event_type_t;

typedef struct event
{
  event_type_t type;
  string data;
}event_t;
class Event
{
  private:
  	Mutex mutex;
  	Cond cond;
  	int listeners;
    queue<event_t> events;
    void wait();
  public:
    Event();
    void sendEvent(event_type_t evt,string &data);
    event_t getEvent();
};

#endif /* EVENT_H_ */

