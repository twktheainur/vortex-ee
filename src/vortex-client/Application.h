#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <Ogre.h>
#include <OIS/OIS.h>
#include "VortexFrameListener.h"
//#include <CEGUI/CEGUI.h>
//#include <OgreCEGUIRenderer.h>

using namespace Ogre;

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

    ~Application();

    void go();

private:
    Root *mRoot;
    OIS::Keyboard *mKeyboard;
    OIS::InputManager *mInputManager;
    //CEGUI::OgreCEGUIRenderer *mRenderer;
    //CEGUI::System *mSystem;
    VortexFrameListener *mListener;
    Camera *mCamera;
    Entity * mPlayer;
    SceneManager *mSceneMgr;
    RenderWindow *win;

    void createRoot();

    void defineResources();

    void setupRenderSystem();

    void createRenderWindow();

    void initializeResourceGroups();

    void setupScene();

    void setupInputSystem();

    // void setupCEGUI();

    void createFrameListener();

    void startRenderLoop();
};

#endif // APPLICATION_H_INCLUDED
