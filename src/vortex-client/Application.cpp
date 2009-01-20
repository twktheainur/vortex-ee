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
        printf("Application Init...\n");
        login(); // on demande à l'utilisateur de se logger
        printf("1\n");
        createRoot(); // on cr�� la racine
        printf("2\n");
        defineResources(); // on d�finit les ressources utilis�es par Ogre
        printf("3\n");
        setupRenderSystem();
        printf("4\n");
        createRenderWindow(); // on cr�� la fen�tre d'affichage de la sc�ne
        printf("5\n");
        initializeResourceGroups(); // on initialise les ressources
        printf("6\n");
        setupScene(); // on installe les �l�ments de la sc�ne
        printf("7\n");
        setupInputSystem();
        printf("8\n");
        //setupCEGUI();
        createFrameListener(); // construction du FrameListener
        printf("9\n");
        startRenderLoop(); // on commence la boucle de rendu
    }

    void Application::login()
    {
        // on fait entrer login + mdp >> Qt
        bitBuffer buff;
        eventManagerEvent.sendEvent(0,buff);
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

      mSceneMgr->setWorldGeometry("maps/PT.bsp"); // chargement de la map
      mSceneMgr->setSkyBox(true, "coucher_soleil"); //chargement de la skybox

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
      SceneNode * playerNode;
      playerNode =mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode", Vector3(-680,160,127));
      mPlayer = mSceneMgr->createEntity( "Player", "man.mesh" );
      mPlayer->setCastShadows(true);
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
        // droite de l'entree
        Light * light1 = mSceneMgr->createLight("L1");
        light1->setType(Light::LT_POINT);
        light1->setPosition(Vector3(-745,-88,200));
        light1->setDiffuseColour(0.5,0.4,0.4);
        light1->setSpecularColour(0.5,0.4,0.4);
        Entity *lampe1 = mSceneMgr->createEntity("lampe1", "lampe.mesh" );
        lampe1->setCastShadows(false);//on ne veux pas que l'objet "lampe" obscursisse la lumière de la lampe...
        SceneNode *lampeNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("lampeNode1", Vector3(-765,-88,200));
        lampeNode1->attachObject(lampe1);
        lampeNode1->yaw(Degree(90));
        lampeNode1->pitch(Degree(180));
        lampeNode1->scale(Vector3(12,9,9));

        //derriere mur tv
        Light * light2 = mSceneMgr->createLight("L2");
        light2->setType(Light::LT_POINT);
        light2->setPosition(Vector3(-88,324,200));
        light2->setDiffuseColour(0.5,0.4,0.4);
        light2->setSpecularColour(0.5,0.4,0.4);
        Entity *lampe2 = mSceneMgr->createEntity("lampe2", "lampe.mesh" );
        SceneNode *lampeNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("lampeNode2", Vector3(-80,324,200));
        lampeNode2->attachObject(lampe2);
        lampeNode1->yaw(Degree(180));
        lampeNode2->scale(Vector3(12,9,9));

        //vers escalier
        Light * light3 = mSceneMgr->createLight("L3");
        light3->setType(Light::LT_POINT);
        light3->setPosition(Vector3(-230,-204,200));
        light3->setDiffuseColour(0.5,0.4,0.4);
        light3->setSpecularColour(0.5,0.4,0.4);
        Entity *lampe3 = mSceneMgr->createEntity("lampe3", "lampe.mesh" );
        SceneNode *lampeNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("lampeNode3", Vector3(-220,-204,200));
        lampeNode3->attachObject(lampe3);
        lampeNode3->yaw(Degree(90));
        lampeNode3->pitch(Degree(180));
        lampeNode3->scale(Vector3(12,9,9));

        //feu
        Light * light4 = mSceneMgr->createLight("L4");
        light4->setType(Light::LT_POINT);
        light4->setPosition(Vector3(768,152,108));
        light4->setDiffuseColour(2, 1, 1);
        light4->setSpecularColour(2, 1, 1);

        //etage
        Light * light5 = mSceneMgr->createLight("L5");
        light5->setType(Light::LT_POINT);
        light5->setPosition(Vector3(776,-204,396));
        light5->setDiffuseColour(0.5,0.3,0.3);
        light5->setSpecularColour(0.5,0.3,0.3);
        Entity *lampe5 = mSceneMgr->createEntity("lampe5", "lampe.mesh" );
        SceneNode *lampeNode5 = mSceneMgr->getRootSceneNode()->createChildSceneNode("lampeNode5", Vector3(776,-204,396));
        lampeNode5->attachObject(lampe5);
        lampeNode5->yaw(Degree(180));
        lampeNode5->scale(Vector3(12,9,9));


        //decoration
        Entity *tv = mSceneMgr->createEntity("TV", "tele.mesh" );
        tv->setCastShadows(true);

        SceneNode *TVNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("TVnode", Vector3(55,392,130));
        TVNode->attachObject(tv);
        TVNode->scale(Vector3(5,4.2,4));
        TVNode->pitch(Degree(90));
        TVNode->yaw(Degree(75));


        Entity *faut1 = mSceneMgr->createEntity("faut1", "fauteuil.mesh" );
        SceneNode *fautNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("fautNode1", Vector3(200,392,60));
        fautNode1->attachObject(faut1);
        fautNode1->scale(Vector3(6,6,6));
        fautNode1->pitch(Degree(90));
        fautNode1->yaw(Degree(270));


        Entity *faut3p1 = mSceneMgr->createEntity("faut3p1", "fauteuil3p.mesh" );
        SceneNode *faut3pNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("faut3pNode1", Vector3(300,392,60));
        faut3pNode1->attachObject(faut3p1);
        faut3pNode1->scale(Vector3(6,6,6));
        faut3pNode1->pitch(Degree(90));
        faut3pNode1->yaw(Degree(270));


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
