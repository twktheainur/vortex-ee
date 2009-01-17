#include "Client.h"
#include "globals.h"
#include <cstdlib>
//Global variables
Event eventManagerEvent;
Event chatManagerEvent;
Event connectionManagerEvent;
Event worldManagerEvent;
int main(int argc, char **argv)
{
  if(argc>1 && strcmp(argv[1],"offline")==0)
  {
    Application app;
    app.go();
  }
  else
    Client client();
	return 0;
}
