/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/

#include "Event.h"
#include <cstdlib>

Event::Event()
{
  clear();
}

void Event::wait()
{
  struct timeval * timer = (struct timeval*)malloc(sizeof(struct timeval));
  timer->tv_usec =25000;
  while(!changed())
    select(0,NULL,NULL,NULL,timer);
}
