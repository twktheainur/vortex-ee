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
#include <string>
#if defined(__WIN32__) || defined(_WIN32)
  #include <winsock.h>
#else
  #include <sys/select.h>
#endif
typedef enum
{
  EV_NONE,
  EV_UPD,
  EV_ADD,
  EV_DEL,
  EV_INC_CHAT,
  EV_OUT_CHAT
}event_t;

class Event
{
  private:
    event_t event;
    string data;
  protected:
    inline void clear(){event=EV_NONE;}
    inline bool changed(){return event!=EV_NONE;data="";}
  public:
    Event(event_t event, string data);
    inline void sendEvent(event_t event){this->event=event;}
    inline event_t getEvent(){event_t ev = event;clear();return ev;}
    inline void setData(string data){this->data=data;}
    inline string getData(){return data;}
    void wait();
};

#endif /* EVENT_H_ */

