#include "Client.h"
#include "globals.h"
#include "ApplicationManager.h"
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <iostream>
//Global variables

Event connectionManagerInEvent;
Event connectionManagerOutEvent;
Event worldManagerEvent;
Event ogreManagerEvent;
int main(int argc, char **argv)
{
  if(argc>1 && strcmp(argv[1],"offline")==0)
  {
    Application app;
    app.go();
  }
  else
  {
    Client client;
  }
	return 0;
}
