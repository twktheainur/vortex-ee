#include "../common/Event.h"
#include "Client.h"

//Global variables
Event main_event;
Event eventManagerEvent;
Event chatManagerEvent;
Event connectionManagerEvent;
Event worldManagerEvent;
int main(int argc, char **argv)
{
  Client client(&main_event);
	return 0;
}
