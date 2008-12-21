#include "../common/Event.h"
#include "Client.h"

//Global variables
Event main_event;
int main(int argc, char **argv)
{
  Client client(&main_event);
	return 0;
}
