#include "Application.h"

    Application::~Application()
    {
      mInputManager->destroyInputObject(mKeyboard);
      OIS::InputManager::destroyInputSystem(mInputManager);

      /*//pour CEGUI :
      delete mRenderer;
      delete mSystem;*/

      delete mListener;
      delete mRoot;
    }

    void Application::go()
    {
        createRoot(); // on cr�� la racine
        defineResources(); // on d�finit les ressources utilis�es par Ogre
        setupRenderSystem();
        createRenderWindow(); // on cr�� la fen�tre d'affichage de la sc�ne
        initializeResourceGroups(); // on initialise les ressources
        setupScene(); // on installe les �l�ments de la sc�ne
        setupInputSystem();
        //setupCEGUI();
        createFrameListener(); // construction du FrameListener
        startRenderLoop(); // on commence la boucle de rendu
    }



    void Application::createRoot()
    {
      #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        mRoot = new Root(macBundlePath() + "/Contents/Resources/plugins.cfg");
      #else
        mRoot = new Root();
      #endif
    }

    void Application::defineResources()
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

    void Application::setupRenderSystem()
    {
      if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())
        throw Exception(52, "User canceled the config dialog!", "Application::setupRenderSystem()");
    }

    void Application::createRenderWindow()
    {
      mRoot->initialise(true, "Vortex");
    }

    void Application::initializeResourceGroups()
    {
      TextureManager::getSingleton().setDefaultNumMipmaps(5);
      ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }

    void Application::setupScene()
    {
      mSceneMgr = mRoot->createSceneManager(ST_INTERIOR, "BspSceneManager");
      mCamera = mSceneMgr->createCamera("Camera"); // on cr�� la camera
      mCamera->setNearClipDistance(5);
      mCamera->pitch(Degree(90)); // On redresse les axes de l'espace pour avoir un d�placement correct de la cam�ra
                                  // (les axes sont invers�s entre le moteur quake et ogre)
      mCamera->setFixedYawAxis(true, Vector3::UNIT_Z); // idem (suite)

      SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Node");
      node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1", Vector3(-850,150,150)); // on place la cam dans l'entr�e

      node->setFixedYawAxis(true, Vector3::UNIT_Z); // on redresse l'axe de la node �galement

      node->attachObject(mCamera); // on attache la camera au noeud

      ////////////////////////////////////////////////////////
      // ici il faudra aussi y attacher le mod�le du joueur //
      ////////////////////////////////////////////////////////

      Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
      mSceneMgr->setWorldGeometry("maps/PT.bsp"); // chargement de la map
      mSceneMgr->setSkyBox(true, "coucher_soleil"); //chargement de la skybox
    }

    void Application::setupInputSystem()
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
           //mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false));
           //mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, false));
       }
       catch (const OIS::Exception &e)
       {
           throw Exception(42, e.eText, "Application::setupInputSystem");
       }
    }

    /*void Application::setupCEGUI()
    {
    }*/

    void Application::createFrameListener()
    {
      mListener = new VortexFrameListener(win, mCamera, mSceneMgr);
      mRoot->addFrameListener(mListener);

      // On ne montre pas les frames de stats
      mListener->showDebugOverlay(true);
    }

    void Application::startRenderLoop()
    {
      mRoot->startRendering();
    }
