/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the	 specific language governing rights and limitations
under the License.*/
#include "Client.h"
#include <cstdlib>
Client::Client(Event * event)
{
	eventManagerEvent = event;
  //connectionManagerThread = new ConnectionManager(this);
  eventManagerThread = new EventManager(event);
  chatManagerThread = new ChatManager(event);
  applicationManagerThread = new ApplicationManager(event);
  applicationManagerThread->join();
}

Client::~Client()
{
	delete connectionManagerThread;
	delete eventManagerThread;
	delete chatManagerThread;
	delete client;
}
