#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <Ogre.h>
#include <OIS/OIS.h>
#include "VortexFrameListener.h"
#include "PersonnagePhysique.h"
#include <MyGUI.h>

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
    Application();
    ~Application();

    void go();

private:
    Root *mRoot;
    OIS::InputManager *mInputManager;
    VortexFrameListener *mListener;
    Camera *mCamera;
    PersonnagePhysique * mPlayer;
    SceneManager *mSceneMgr;
    RenderWindow *win;

    MyGUI::Gui * mGUI;

    void login();

    void createRoot();

    void defineResources();

    void setupRenderSystem();

    void createRenderWindow();

    void initializeResourceGroups();

    void chooseSceneManager();

    void setupScene();

    void setupInputSystem();

    void setupMyGUI();

    void createFrameListener();

    void startRenderLoop();
};

#endif // APPLICATION_H_INCLUDED
