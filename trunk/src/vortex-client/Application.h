#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <Ogre.h>
#include <OIS/OIS.h>
#include "VortexFrameListener.h"
#include "OgreReferenceAppLayer.h"//pour la gestion des collisions/gravité
#include "PersonnagePhysique.h"//pour les collisions / la gravité
//#include <CEGUI/CEGUI.h>
//#include <OgreCEGUIRenderer.h>

using namespace Ogre;
using namespace OgreRefApp;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>

// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
std::string macBundlePath()
{
    char path[1024];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    assert(mainBundle);

    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    assert(mainBundleURL);

    CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
    assert(cfStringRef);

    CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

    CFRelease(mainBundleURL);
    CFRelease(cfStringRef);

    return std::string(path);
}
#endif



class Application
{
public:
    Application();
    ~Application();

    void go();

private:
    Root *mRoot;
    //OIS::Keyboard *mKeyboard; // ca sert a rien, on l'a déjà dans le frameListener
    OIS::InputManager *mInputManager;
    //CEGUI::OgreCEGUIRenderer *mRenderer;
    //CEGUI::System *mSystem;
    VortexFrameListener *mListener;
    World* mWorld;//pour la gestion des collisons/gravité
    Camera *mCamera;
    //Entity* mPLayer;
    PersonnagePhysique * mPlayer;
    SceneManager *mSceneMgr;
    RenderWindow *win;

    void login();

    void createRoot();

    void defineResources();

    void setupRenderSystem();

    void createRenderWindow();

    void initializeResourceGroups();

    void chooseSceneManager();

    void createWorld();//pour la gestion des collisions/gravite

    void setupScene();

    void setupInputSystem();

    // void setupCEGUI();

    void createFrameListener();

    void startRenderLoop();
};

#endif // APPLICATION_H_INCLUDED
