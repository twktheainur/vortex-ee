#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "ExitListener.h"

using namespace Ogre;

class Application{
	public:
		~Application();
		void go();

	private:
    Root *mRoot;
    OIS::Keyboard *mKeyboard;
    OIS::InputManager *mInputManager;
    //CEGUI::OgreCEGUIRenderer *mRenderer;
    //CEGUI::System *mSystem;
    ExitListener *mListener;

		//CEGUI::OgreCEGUIRenderer *mRenderer;
		//CEGUI::System *mSystem;

		void createRoot();
		void defineResources();
		void setupRenderSystem();
		void createRenderWindow();
		void initializeResourceGroups();		void setupScene();
		void setupInputSystem();
		//void setupCEGUI();
		void createFrameListener();
		void startRenderLoop();
};

#endif
