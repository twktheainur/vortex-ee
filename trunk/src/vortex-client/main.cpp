#include <Ogre.h>
#include <OIS/OIS.h>

<<<<<<< .mine
#include "ExitListener.h"
#include "Application.h"
=======
#include "Application.h"
#include "ExitListener.h"
>>>>>>> .r39

using namespace Ogre;

int main(int argc, char **argv){	try{
		Application app;
		app.go();}
    
	catch(Exception& e){
		fprintf(stderr, "An exception has occurred: %s\n",
		e.getFullDescription().c_str());}

	return 0;}
