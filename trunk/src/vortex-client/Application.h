#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "ExitListener.h"

using namespace Ogre;

class Application
{
public:
    void go();
    ~Application();

private:
    Root *mRoot;
    OIS::Keyboard *mKeyboard;
    OIS::InputManager *mInputManager;
    SceneManager *mSceneMgr;
    Camera *mCamera;
    RenderWindow *win;
    //CEGUI::OgreCEGUIRenderer *mRenderer;
    //CEGUI::System *mSystem;
    ExitListener *mListener;

    void createRoot();
    void defineResources();
    void setupRenderSystem();
    void createRenderWindow();
    void initializeResourceGroups();    void setupScene();
    void setupInputSystem();
//    void setupCEGUI();
    void createFrameListener();
    void startRenderLoop();
};

#endif
