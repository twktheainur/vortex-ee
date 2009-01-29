#include "Client.h"
#include "globals.h"
#include "ApplicationManager.h"
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <iostream>
#include <fstream>
//Global variables

Event connectionManagerInEvent;
Event connectionManagerOutEvent;
Event worldManagerEvent;
Event ogreManagerEvent;
std::ofstream flog;

string idClientGlobal = "";
bool continuer = true;

int main(int argc, char **argv)
{
  flog.open("Cli.log");
  if(argc>1 && strcmp(argv[1],"offline")==0)
  {
    Application app;
    app.go();
  }
  else
  {
    try
    {
      Client client;
    }
    catch(vortex::Exception  * e)
    {
      flog <<e->what() << endl;
    }
  }
	return 0;
}
