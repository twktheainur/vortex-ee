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
      mCamera = mSceneMgr->createCamera("Camera"); // on cree la camera
      mCamera->setNearClipDistance(5);
      //mCamera->pitch(Degree(90)); // On redresse les axes de l'espace pour avoir un deplacement correct de la camera
                                  // (les axes sont inverses entre le moteur quake et ogre)
      mCamera->setFixedYawAxis(true, Vector3::UNIT_Z); // idem (suite)
      SceneNode * playerNode;
      playerNode =mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode", Vector3(-680,160,127));
      mPlayer = mSceneMgr->createEntity( "Player", "man.mesh" );
      playerNode->pitch(Degree(90));
      playerNode->yaw(Degree(90));
      playerNode->scale(Vector3(2,2,2));
      playerNode->setFixedYawAxis(true, Vector3::UNIT_Z); // on redresse l'axe de la node �galement
      playerNode->attachObject(mPlayer); // on attache le modèle au noeud

      SceneNode *camNode;
      camNode = playerNode->createChildSceneNode("CamNode", Vector3(0,40,-73)); // on place la cam dans l'entree
      camNode->pitch(Degree(200));
      camNode->roll(Degree(180));
      camNode->setFixedYawAxis(true, Vector3::UNIT_Z); // on redresse l'axe de la node �galement
      //mCamera->setAutoTracking(true, playerNode);

      camNode->attachObject(mCamera); // on attache la camera au noeud



      Light * light = mSceneMgr->createLight("L1");
      light->setType(Light::LT_POINT);
      light->setPosition(Vector3(-600,160,200));

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
    }

    /*void Application::setupCEGUI()
    {
    }*/

    void Application::createFrameListener()
    {
      mListener = new VortexFrameListener(win, mCamera,mPlayer, mSceneMgr);
      mRoot->addFrameListener(mListener);

      // On ne montre pas les frames de stats
      mListener->showDebugOverlay(false);
    }

    void Application::startRenderLoop()
    {
      mRoot->startRendering();
    }
