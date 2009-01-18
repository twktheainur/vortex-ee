/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/

#include "Event.h"
//#include "Cond.h"
#include <cstdlib>


Event::Event()
     :semaphore(0)
{}

void Event::wait()
{
  try
  {
     semaphore.wait();
  }
  catch(vortex::Exception * e)
  {
    throw;
  }
}

void Event::sendEvent(int evt, bitBuffer & data)
{
  event_data_t event	;
  event.data=data;
  event.type=evt;
  mutex.lock();
  events.push(event);
  while(semaphore.getValue()<0)
    semaphore.post();
  mutex.unlock();
}

event_data_t Event::getEvent()
{
  event_data_t event;
  mutex.lock();
  listeners++;
  if(events.empty())
  {
  	mutex.unlock();
  	wait();
  	mutex.lock();
  }
  event =events.front();
  if(listeners<=1)
  {
  	events.pop();
  }
  listeners--;
  mutex.unlock();
  Event::usleep(10);
  return event;
}
