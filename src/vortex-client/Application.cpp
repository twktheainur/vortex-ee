#include "Application.h"

    Application::Application()
    {
      mListener = 0;
      mRoot = 0;
      mWorld = 0;
    }

    Application::~Application()
    {
      // mInputManager->destroyInputObject(mKeyboard); //la ligne de code qui m'a fait perdre le plus de temps dans ma vie...
      // A NE PAS DECOMMENTER (sinon, ca va planter au moment de quitter l'application...)

      OIS::InputManager::destroyInputSystem(mInputManager);

      //pour CEGUI :
      //delete mRenderer;
      //delete mSystem;

      if(mListener)
      {
        delete mListener;
      }
      if(mWorld)
      {
        delete mWorld;
      }
      if(mRoot)
      {
        delete mRoot;
      }

      if(mGUI)
      {
        //mGUI->shutdown();//cette ligne provoque un crash
        delete mGUI;
        //mGUI = 0;
      }
    }

    void Application::go()
    {
        createRoot(); // on cr�� la racine
        defineResources(); // on d�finit les ressources utilis�es par Ogre
        setupRenderSystem();
        createRenderWindow(); // on cr�� la fen�tre d'affichage de la sc�ne
        initializeResourceGroups(); // on initialise les ressources
        chooseSceneManager();
        createWorld();//pour la gestion des collisions/gravité
        //doit se situer APRES l'initialisation du SceneManager (methode chooseSceneManager() )
        setupScene(); // on installe les �l�ments de la sc�ne + initialisation du SceneManager
        setupInputSystem();
        setupMyGUI();
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
      if (!mRoot->restoreConfig())
      {
        if(!mRoot->showConfigDialog())
        {
          throw Exception(52, "User canceled the config dialog!", "Application::setupRenderSystem()");
        }
      }
    }

    void Application::chooseSceneManager()
    {
      mSceneMgr = mRoot->createSceneManager(ST_INTERIOR, "BspSceneManager");
    }

    void Application::createWorld(void)
    {
        mWorld = new World(mSceneMgr, World::WT_REFAPP_BSP);
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
      mWorld->getSceneManager()->setWorldGeometry("maps/PT.bsp");//chargement de la map
      //mSceneMgr->setWorldGeometry("maps/PT.bsp"); // chargement de la map
      mSceneMgr->setSkyBox(true, "coucher_soleil"); //chargement de la skybox

      #ifdef COLLISIONS_GRAVITE_ACTIVEES//défini dans VortexFrameListener.h
        //mWorld->setGravity(Vector3(0, 0, -100));
      #endif

      mCamera = mSceneMgr->createCamera("Camera"); // on cree la camera
      mCamera->setNearClipDistance(5);
      mSceneMgr->setAmbientLight(ColourValue(0,0,0));//luminosité ambiente assez faible
      //permet de voir les ombres, et les effets des lumières des lampes, sans etre completement dans le noir

      //mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
      //a reactiver plus tard (quand on aura les collisions par exemple)
      //La seule technique d'ombre qui ait projeté des ombres sur la map bsp est SHADOWTYPE_STENCIL_MODULATIVE
      //par contre avec des ombres de type STENCIL, le nombre de FPS descend à 60 sur une 8800GTX OC

      //mCamera->pitch(Degree(90)); // On redresse les axes de l'espace pour avoir un deplacement correct de la camera
                                  // (les axes sont inverses entre le moteur quake et ogre)
      mCamera->setFixedYawAxis(true, Vector3::UNIT_Z); // idem (suite)
      //SceneNode * playerNode;
      //playerNode =mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode", Vector3(-680,160,127));
      //mPlayer = mSceneMgr->createEntity( "Player", "man.mesh" );
      mPlayer = mWorld->createPersonnage("Player", Vector3(10,10,10), Vector3(-680,160,127));//nom, dimensions (x,y,z), position (x,y,z)
      // les 2 derniers paramètres sont facultatifs

      //mPlayer->getEntity()->setCastShadows(true);

      //le code suivant se retrouve dans PersonnagePhysique
      //playerNode->pitch(Degree(90));
      mPlayer->getSceneNode()->pitch(Degree(90));
      //playerNode->yaw(Degree(90));
      mPlayer->getSceneNode()->yaw(Degree(90));
      //playerNode->scale(Vector3(2,2,2));
      mPlayer->getSceneNode()->scale(Vector3(2,2,2));
      //playerNode->setFixedYawAxis(true, Vector3::UNIT_Z); // on redresse l'axe de la node �galement
      mPlayer->getSceneNode()->setFixedYawAxis(true, Vector3::UNIT_Z); // on redresse l'axe de la node �galement
      //playerNode->attachObject(mPlayer); // on attache le modèle au noeud

      #ifdef COLLISIONS_GRAVITE_ACTIVEES
        mPlayer->setCollisionEnabled(true);
        mPlayer->setDynamicsEnabled(true);
      #endif

      SceneNode *camRotNode;
      //camRotNode=playerNode->createChildSceneNode("CamRotNode", Vector3(0,20,0)); //SceneNode autour duquel la caméra va tourner (légèrement plus haut que le playerNode)
      camRotNode=mPlayer->getSceneNode()->createChildSceneNode("CamRotNode", Vector3(0,20,0)); //SceneNode autour duquel la caméra va tourner (légèrement plus haut que le playerNode)
      //situé au même endroit que le playerNode, (la caméra tourne autour du player en apparence) mais peut subir un "pitch" (inclinaison verticale) sans modifier celle du playerNode
      SceneNode *camNode;
      camNode = camRotNode->createChildSceneNode("CamNode", Vector3(0,0,-60)); // on place la camera derriere le joueur
      camNode->setDirection(0,0,1);//pour que la camera regarde le joueur, pas la porte du fond...
      camRotNode->pitch(Degree(20));//la caméra regarde légèrement par au-dessus
      //camNode->setFixedYawAxis(true, Vector3::UNIT_Z); // on redresse l'axe de la node �galement

      camNode->attachObject(mCamera); // on attache la camera au noeud

       //lumieres
        // droite de l'entree
        Light * light1 = mSceneMgr->createLight("L1");
        light1->setType(Light::LT_POINT);
        light1->setPosition(Vector3(-765,-88,200));
        light1->setDiffuseColour(0.5,0.45,0.45);
        light1->setSpecularColour(0.5,0.45,0.45);
        Entity *lampe1 = mSceneMgr->createEntity("lampe1", "lampe.mesh" );
        lampe1->setCastShadows(false);//on ne veux pas que l'objet "lampe" obscursisse la lumière de la lampe...
        SceneNode *lampeNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("lampeNode1", Vector3(-765,-88,200));
        lampeNode1->attachObject(lampe1);
        lampeNode1->yaw(Degree(270));
        lampeNode1->pitch(Degree(180));
        lampeNode1->scale(Vector3(12,9,9));

        //derriere mur tv
        Light * light2 = mSceneMgr->createLight("L2");
        light2->setType(Light::LT_POINT);
        light2->setPosition(Vector3(-80,324,200));
        light2->setDiffuseColour(0.5,0.45,0.45);
        light2->setSpecularColour(0.5,0.45,0.45);
        Entity *lampe2 = mSceneMgr->createEntity("lampe2", "lampe.mesh" );
        SceneNode *lampeNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("lampeNode2", Vector3(-79,324,200));
        lampeNode2->attachObject(lampe2);
        lampeNode2->yaw(Degree(270));
        lampeNode2->scale(Vector3(12,9,9));

        //vers escalier
        Light * light3 = mSceneMgr->createLight("L3");
        light3->setType(Light::LT_POINT);
        light3->setPosition(Vector3(-220,-204,200));
        light3->setDiffuseColour(0.5,0.45,0.45);
        light3->setSpecularColour(0.5,0.45,0.45);
        Entity *lampe3 = mSceneMgr->createEntity("lampe3", "lampe.mesh" );
        SceneNode *lampeNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("lampeNode3", Vector3(-225,-204,200));
        lampeNode3->attachObject(lampe3);
        lampeNode3->yaw(Degree(270));
        lampeNode3->pitch(Degree(180));
        lampeNode3->scale(Vector3(12,9,9));

        //feu
        Light * light4 = mSceneMgr->createLight("L4");
        light4->setType(Light::LT_POINT);
        light4->setPosition(Vector3(768,152,108));
        light4->setDiffuseColour(1.5,1.2,1.2);
        light4->setSpecularColour(1,0.6,0.6);

        //etage
        Light * light5 = mSceneMgr->createLight("L5");
        light5->setType(Light::LT_POINT);
        light5->setPosition(Vector3(826,-225,396));
        light5->setDiffuseColour(0.5,0.45,0.45);
        light5->setSpecularColour(0.5,0.4,0.4);
        Entity *lampe5 = mSceneMgr->createEntity("lampe5", "lampe.mesh" );
        SceneNode *lampeNode5 = mSceneMgr->getRootSceneNode()->createChildSceneNode("lampeNode5", Vector3(826,-225,396));
        lampeNode5->attachObject(lampe5);
        lampeNode5->yaw(Degree(270));
        lampeNode5->scale(Vector3(12,9,9));


        // sous l'escalier
        Light * light6 = mSceneMgr->createLight("L6");
        light6->setType(Light::LT_POINT);
        light6->setPosition(Vector3(380,-253,170));
        light6->setDiffuseColour(0.2,0.15,0.15);
        light6->setSpecularColour(0.2,0.15,0.15);
        Entity *lampe6 = mSceneMgr->createEntity("lampe6", "lampe.mesh" );
        SceneNode *lampeNode6 = mSceneMgr->getRootSceneNode()->createChildSceneNode("lampeNode6", Vector3(380,-253,170));
        lampeNode6->attachObject(lampe6);
        lampeNode6->yaw(Degree(270));
        lampeNode6->pitch(Degree(270));
        lampeNode6->scale(Vector3(12,9,9));


        //decoration
        //television
        Entity *tv = mSceneMgr->createEntity("TV", "tele.mesh" );
        tv->setCastShadows(true);
        SceneNode *TVNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("TVnode", Vector3(55,392,120));
        TVNode->attachObject(tv);
        TVNode->scale(Vector3(4,3.7,3));
        TVNode->pitch(Degree(90));
        TVNode->yaw(Degree(75));

        // devant la télé
        Entity *faut1 = mSceneMgr->createEntity("faut1", "fauteuil.mesh" );
        SceneNode *fautNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("fautNode1", Vector3(450,485,60));
        fautNode1->attachObject(faut1);
        fautNode1->scale(Vector3(6,6,6));
        fautNode1->pitch(Degree(90));
        fautNode1->yaw(Degree(290));

        Entity *faut2 = mSceneMgr->createEntity("faut2", "fauteuil.mesh" );
        SceneNode *fautNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("fautNode2", Vector3(420,170,60));
        fautNode2->attachObject(faut2);
        fautNode2->scale(Vector3(6,6,6));
        fautNode2->pitch(Degree(90));
        fautNode2->yaw(Degree(250));

        Entity *faut3p1 = mSceneMgr->createEntity("faut3p1", "fauteuil3p.mesh" );
        SceneNode *faut3pNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("faut3pNode1", Vector3(460,325,60));
        faut3pNode1->attachObject(faut3p1);
        faut3pNode1->scale(Vector3(6,6,6));
        faut3pNode1->pitch(Degree(90));
        faut3pNode1->yaw(Degree(268));

        //fauteuil cheminée
        Entity *faut3 = mSceneMgr->createEntity("faut3", "fauteuil.mesh" );
        SceneNode *fautNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("fautNode3", Vector3(600,-80,60));
        fautNode3->attachObject(faut3);
        fautNode3->scale(Vector3(6,6,6));
        fautNode3->pitch(Degree(90));
        fautNode3->yaw(Degree(55));

        // table de l'autre cote du mur derriere l'escalier
        Entity *faut4 = mSceneMgr->createEntity("faut4", "fauteuil.mesh" );
        SceneNode *fautNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode("fautNode4", Vector3(-290,-180,118));
        fautNode4->attachObject(faut4);
        fautNode4->scale(Vector3(6,6,6));
        fautNode4->pitch(Degree(90));
        fautNode4->yaw(Degree(245));

        Entity *faut5 = mSceneMgr->createEntity("faut5", "fauteuil.mesh" );
        SceneNode *fautNode5 = mSceneMgr->getRootSceneNode()->createChildSceneNode("fautNode5", Vector3(-290,10,118));
        fautNode5->attachObject(faut5);
        fautNode5->scale(Vector3(6,6,6));
        fautNode5->pitch(Degree(90));
        fautNode5->yaw(Degree(282));

        Entity *faut3p2 = mSceneMgr->createEntity("faut3p2", "fauteuil3p.mesh" );
        SceneNode *faut3pNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("faut3pNode2", Vector3(-460,-50,118));
        faut3pNode2->attachObject(faut3p2);
        faut3pNode2->scale(Vector3(6,6,6));
        faut3pNode2->pitch(Degree(90));
        faut3pNode2->yaw(Degree(78));


        //fauteuils table derriere mur tv
        Entity *faut6 = mSceneMgr->createEntity("faut6", "fauteuil.mesh" );
        SceneNode *fautNode6 = mSceneMgr->getRootSceneNode()->createChildSceneNode("fautNode6", Vector3(-500,430,118));
        fautNode6->attachObject(faut6);
        fautNode6->scale(Vector3(6,6,6));
        fautNode6->pitch(Degree(90));
        fautNode6->yaw(Degree(50));

        Entity *faut7 = mSceneMgr->createEntity("faut7", "fauteuil.mesh" );
        SceneNode *fautNode7 = mSceneMgr->getRootSceneNode()->createChildSceneNode("fautNode7", Vector3(-530,260,118));
        fautNode7->attachObject(faut7);
        fautNode7->scale(Vector3(6,6,6));
        fautNode7->pitch(Degree(90));
        fautNode7->yaw(Degree(100));

        Entity *faut8 = mSceneMgr->createEntity("faut8", "fauteuil.mesh" );
        SceneNode *fautNode8 = mSceneMgr->getRootSceneNode()->createChildSceneNode("fautNode8", Vector3(-230,350,118));
        fautNode8->attachObject(faut8);
        fautNode8->scale(Vector3(6,6,6));
        fautNode8->pitch(Degree(90));
        fautNode8->yaw(Degree(280));

        Entity *faut9 = mSceneMgr->createEntity("faut9", "fauteuil.mesh" );
        SceneNode *fautNode9 = mSceneMgr->getRootSceneNode()->createChildSceneNode("fautNode9", Vector3(-350,470,118));
        fautNode9->attachObject(faut9);
        fautNode9->scale(Vector3(6,6,6));
        fautNode9->pitch(Degree(90));


      Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
      vp=NULL;
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

    void Application::setupMyGUI()
    {
        mGUI = new MyGUI::Gui();
        mGUI->initialise(win);
    }

    void Application::createFrameListener()
    {
      mListener = new VortexFrameListener(win, mCamera,mPlayer, mSceneMgr, mGUI);
      mRoot->addFrameListener(mListener);

    }

    void Application::startRenderLoop()
    {
      mRoot->startRendering();
    }
