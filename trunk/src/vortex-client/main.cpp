#include <Ogre.h>
#include <OIS/OIS.h>


using namespace Ogre;

#if defined(APPLE)
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

class ExitListener : public FrameListener
{
public:
    ExitListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr,OIS::Keyboard *keyboard)
        : mKeyboard(keyboard)
    {
        this->mSceneMgr = mSceneMgr;   // The current SceneManager
        this->mCamNode =cam->getParentSceneNode();   // The SceneNode the camera is currently attached to

        // set the rotation and move speed
        mRotate = 0.13;
        mMove = 250;

        // continue rendering
        mContinue = true;
    }


    bool frameStarted(const FrameEvent &evt)
    {
      //  if(mMouse)
        //    mMouse->capture();
        if(mKeyboard)
            mKeyboard->capture();
        mCamNode->translate(mDirection * evt.timeSinceLastFrame, Node::TS_LOCAL);
        return mContinue;
    }

    // MouseListener
    //bool mouseMoved(const OIS::MouseEvent &e) { return true; }
    //bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) { return true; }
    //bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) { return true; }

    // KeyListener
    bool keyPressed(const OIS::KeyEvent &e)
    {
       switch (e.key)
       {
           case OIS::KC_ESCAPE:
               mContinue = false;
               break;
           case OIS::KC_UP:
       case OIS::KC_W:
           mDirection.z = -mMove;
           break;

       case OIS::KC_DOWN:
       case OIS::KC_S:
           mDirection.z = mMove;
           break;

       case OIS::KC_LEFT:
       case OIS::KC_A:
           mDirection.x = -mMove;
           break;

       case OIS::KC_RIGHT:
       case OIS::KC_D:
           mDirection.x = mMove;
           break;

       case OIS::KC_PGDOWN:
       case OIS::KC_E:
           mDirection.y = -mMove;
           break;

       case OIS::KC_PGUP:
       case OIS::KC_Q:
           mDirection.y = mMove;
           break;
           default:
               break;
       }
       return mContinue;
    }
    bool keyReleased(const OIS::KeyEvent &e)
    {
           switch (e.key)
       {
       case OIS::KC_UP:
       case OIS::KC_W:
           mDirection.z = 0;
           break;

       case OIS::KC_DOWN:
       case OIS::KC_S:
           mDirection.z = 0;
           break;

       case OIS::KC_LEFT:
       case OIS::KC_A:
           mDirection.x = 0;
           break;

       case OIS::KC_RIGHT:
       case OIS::KC_D:
           mDirection.x = 0;
           break;

       case OIS::KC_PGDOWN:
       case OIS::KC_E:
           mDirection.y = 0;
           break;

       case OIS::KC_PGUP:
       case OIS::KC_Q:
           mDirection.y = 0;
           break;

       default:
           break;
       } // switch
       return true;
    }
protected:
    Real mRotate;          // The rotate constant
    Real mMove;            // The movement constant

    SceneManager *mSceneMgr;   // The current SceneManager
    SceneNode *mCamNode;   // The SceneNode the camera is currently attached to

    OIS::Keyboard *mKeyboard;

    bool mContinue;        // Whether to continue rendering or not
    Vector3 mDirection;     // Value to move in the correct direction
};

class Application
{
public:
    void go()
    {
        createRoot();
        defineResources();
        setupRenderSystem();
        createRenderWindow();
        initializeResourceGroups();
        setupScene();
        //setupInputSystem();
        //setupCEGUI();
        createFrameListener();
        startRenderLoop();
    }

    ~Application()
    {
      mInputManager->destroyInputObject(mKeyboard);
      OIS::InputManager::destroyInputSystem(mInputManager);

      /*//pour CEGUI :
      delete mRenderer;
      delete mSystem;*/

      delete mListener;
      delete mRoot;
    }

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

    void createRoot()
    {
      #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        mRoot = new Root(macBundlePath() + "/Contents/Resources/plugins.cfg");
      #else
        mRoot = new Root();
      #endif
    }

    void defineResources()
    {
      String secName, typeName, archName;
      ConfigFile cf;
      #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        cf.load(macBundlePath() + "/Contents/Resources/resources.cfg");
      #else
        cf.load("resources.cfg");
      #endif
      ConfigFile::SectionIterator seci = cf.getSectionIterator();
        while (seci.hasMoreElements())
        {
          secName = seci.peekNextKey();
          ConfigFile::SettingsMultiMap *settings = seci.getNext();
          ConfigFile::SettingsMultiMap::iterator i;
          for (i = settings->begin(); i != settings->end(); ++i)
          {
            typeName = i->first;
            archName = i->second;
            #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
              ResourceGroupManager::getSingleton().addResourceLocation( String(macBundlePath() + "/" + archName), typeName, secName);
            #else
              ResourceGroupManager::getSingleton().addResourceLocation( archName, typeName, secName);
            #endif
          }
        }
    }

    void setupRenderSystem()
    {
      if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())
        throw Exception(52, "User canceled the config dialog!", "Application::setupRenderSystem()");
    }

    void createRenderWindow()
    {
      mRoot->initialise(true, "Vortex");
    }

    void initializeResourceGroups()
    {
      TextureManager::getSingleton().setDefaultNumMipmaps(5);
      ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }

    void setupScene()
    {
      mSceneMgr = mRoot->createSceneManager(ST_INTERIOR, "BspSceneManager");
      mCamera = mSceneMgr->createCamera("Camera");
      mCamera->pitch(Degree(90)); // On redresse les axes de l'espace pour avoir un d�placement correct de la cam�ra (les axes sont invers�s entre quake et ogre)
      mCamera->setFixedYawAxis(true, Vector3::UNIT_Z);
      Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
      mSceneMgr->setWorldGeometry("maps/PT.bsp");//chargmement de la map
      //mSceneMgr->setSkyBox(true, "cloudy_noon"); //chargement de la skybox
    }

    void setupInputSystem()
    {
      size_t windowHnd = 0;
      std::ostringstream windowHndStr;
      OIS::ParamList pl;
      win = mRoot->getAutoCreatedWindow();

      win->getCustomAttribute("WINDOW", &windowHnd);
      windowHndStr << windowHnd;
      pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
      mInputManager = OIS::InputManager::createInputSystem(pl);

      try
       {
           mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false));
           //mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, false));
       }
       catch (const OIS::Exception &e)
       {
           throw Exception(42, e.eText, "Application::setupInputSystem");
       }
    }

    /*void setupCEGUI()
    {
    }*/

    void createFrameListener()
    {
      mListener = new ExitListener(win,mCamera,mSceneMgr,mKeyboard);
      mRoot->addFrameListener(mListener);
    }

    void startRenderLoop()
    {
      mRoot->startRendering();
    }
};

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
    try
    {
        Application app;
        app.go();
    }
    catch(Exception& e)
    {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n",
            e.getFullDescription().c_str());
#endif
    }

    return 0;
}
