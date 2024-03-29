/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/

#ifndef CLIENTMANAGEROUT_H_
#define CLIENTMANAGEROUT_H_
#include "globals.h"
#include "../common/Thread.h"
#include "../common/Event.h"
#include "../common/TCPSocket.h"

class ClientManagerOut : public Thread
{
private:
  Event event;
  TCPSocket socket;

  void execute(void * arg);
public:
	ClientManagerOut(TCPSocket sock);
	~ClientManagerOut();
  inline Event getEvent(){return event;}
};

#endif /* CLIENTMANAGER_H_ */
