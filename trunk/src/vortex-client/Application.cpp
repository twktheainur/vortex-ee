#include "Application.h"

    Application::~Application()
    {
      // mInputManager->destroyInputObject(mKeyboard); //la ligne de code qui m'a fait perdre le plus de temps dans ma vie...
      // A NE PAS DECOMMENTER (sinon, ca va planter au moment de quitter l'application...)

      OIS::InputManager::destroyInputSystem(mInputManager);

      //pour CEGUI :
      //delete mRenderer;
      //delete mSystem;

      delete mListener;
      delete mRoot;
    }

    void Application::go()
    {
        login(); // on demande à l'utilisateur de se logger
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

    void Application::login()
    {
        // on fait entrer login + mdp
        // on envoie le login event a l'event manager
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

      SceneNode *camRotNode;
      camRotNode=playerNode->createChildSceneNode("CamRotNode", Vector3(0,20,0)); //SceneNode autour duquel la caméra va tourner (légèrement plus haut que le playerNode)
      //situé au même endroit que le playerNode, (la caméra tourne autour du player en apparence) mais peut subir un "pitch" (inclinaison verticale) sans modifier celle du playerNode
      SceneNode *camNode;
      camNode = camRotNode->createChildSceneNode("CamNode", Vector3(0,0,-60)); // on place la camera derriere le joueur
      camNode->setDirection(0,0,1);//pour que la camera regarde le joueur, pas la porte du fond...
      camRotNode->pitch(Degree(20));//la caméra regarde légèrement par au-dessus
      //camNode->setFixedYawAxis(true, Vector3::UNIT_Z); // on redresse l'axe de la node �galement

      camNode->attachObject(mCamera); // on attache la camera au noeud

        //lumieres
        Light * light1 = mSceneMgr->createLight("L1");
        light1->setType(Light::LT_POINT);
        light1->setPosition(Vector3(-728,-88,188));
        light1->setDiffuseColour(0.5,0.4,0.4);
        light1->setSpecularColour(0.5,0.4,0.4);
        Entity *lampe1 = mSceneMgr->createEntity("lampe1", "lampe/lampe.mesh" );
        SceneNode *lampeNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("lampeNode1", Vector3(-728,-88,188));
        lampeNode1->attachObject(lampe1);

        Light * light2 = mSceneMgr->createLight("L2");
        light2->setType(Light::LT_POINT);
        light2->setPosition(Vector3(-88,324,200));
        light2->setDiffuseColour(0.5,0.4,0.4);
        light2->setSpecularColour(0.5,0.4,0.4);

        Light * light3 = mSceneMgr->createLight("L3");
        light3->setType(Light::LT_POINT);
        light3->setPosition(Vector3(-192,-204,196));
        light3->setDiffuseColour(0.5,0.4,0.4);
        light3->setSpecularColour(0.5,0.4,0.4);

        Light * light4 = mSceneMgr->createLight("L4");
        light4->setType(Light::LT_POINT);
        light4->setPosition(Vector3(768,152,108));
        light4->setDiffuseColour(2, 1, 1);
        light4->setSpecularColour(2, 1, 1);

        Light * light5 = mSceneMgr->createLight("L5");
        light5->setType(Light::LT_POINT);
        light5->setPosition(Vector3(776,-204,396));
        light5->setDiffuseColour(0.5,0.3,0.3);
        light5->setSpecularColour(0.5,0.3,0.3);

        //decoration
        Entity *tv = mSceneMgr->createEntity("TV", "tele/tele.mesh" );
        SceneNode *TVNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("TVnode", Vector3(55,392,144));
        TVNode->attachObject(tv);



      Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
      vp=NULL;
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

    }

    void Application::startRenderLoop()
    {
      mRoot->startRendering();
    }
