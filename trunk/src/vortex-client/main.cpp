#include <Ogre.h>
#include <OIS/OIS.h>

#include "ExitListener.h"
#include "Application.h"

using namespace Ogre;

int main(int argc, char **argv){	try{
		Application app;
		app.go();}
    
	catch(Exception& e){
		fprintf(stderr, "An exception has occurred: %s\n",
		e.getFullDescription().c_str());}

	return 0;}
