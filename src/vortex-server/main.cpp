/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#define _REENTRANT
#include "Server.h"
#include "../common/Event.h"
#include "../common/Mutex.h"
//#include "../common/BinBitSet.h"
#include "globals.h"
#include <cstring>
#include <cstdlib>

extern "C"
{
  #include <time.h>
}

//Global variables
Event clientManagerEvent;
Event connectionManagerEvent;
Event worldManagerEvent;
int main(int argc, char **argv)
{
  Server server;
  return EXIT_SUCCESS;
}
