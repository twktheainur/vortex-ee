#include "VortexFrameListener.h"
#include "../common/bitBuffer.h"

    VortexFrameListener::VortexFrameListener(RenderWindow* win, Camera* cam,PersonnagePhysique * player, SceneManager *sceneMgr, MyGUI::Gui * mGUI)
    {
      mInputManager = InputManager::getSingletonPtr();
      mInputManager->initialise( win );
      mInputManager->addKeyListener( this, "VortexFrameListener" );
      mInputManager->addMouseListener( this, "VortexFrameListener" );
      mMouse = mInputManager->getMouse();
      mKeyboard = mInputManager->getKeyboard();

      this->mGUI = mGUI;

        winChat = MyGUI::LayoutManager::getInstance().load("chat.layout");

        winAccueil = MyGUI::LayoutManager::getInstance().load("accueil.layout");

        winLaunchInter = MyGUI::LayoutManager::getInstance().load("launchInterface.layout");
        showWindow(1,false);

        // set callback
        MyGUI::ButtonPtr buttonOK = mGUI->findWidget<MyGUI::Button>("buttonOK");
        buttonOK->eventMouseButtonClick = MyGUI::newDelegate(this, &VortexFrameListener::hideAccueil);


        // Populate the camera and scene manager containers
        mCamNode = cam->getParentSceneNode();
        mCamRotNode = mCamNode->getParentSceneNode();
        mPlayer = player;
        //mPlayerNode=player->getParentSceneNode();
        mSceneMgr = sceneMgr;

        mAnimationState = mPlayer->getEntity()->getAnimationState("marcheAvant");
        mAnimationState->setLoop(true);
        mAnimationState->setEnabled(false);

        mRotate = 0.2; // valeur � changer pour la sensibilit� de la souris
        mMove = 100; // vitesse de d�placement

        // bool�en d�finissant si on continue ou non le rendu des images
        mContinue = true;

        mDirection = Vector3::ZERO;

        changement = false; // on initialise le booleen de changement
        iteration = 5;

        idClient = "Client";

        lauchIntShow = false;
    }

    void VortexFrameListener::hideAccueil(MyGUI::WidgetPtr _sender)
    {
        winAccueil[0]->hide();
    }

    void VortexFrameListener::showWindow(int window, bool show)
    {
        switch (window)
        {
            case 1: //winLaunchInter
                if (show) winLaunchInter[0]->show();
                else winLaunchInter[0]->hide();
            break;

            default:
            break;
        }
    }

    bool VortexFrameListener::frameEnded(const FrameEvent &evt)
    {
        mGUI->injectFrameEntered(evt.timeSinceLastFrame);

        // on declare les variables de position et direction qui nous servirons plusieurs fois par la suite
        Vector3 pos;
        float posX;
        float posY;
        float posZ;
        float dirX = 0;
        float dirY = 0;

        // d'abord on capture les actions effectu�es � la souris et au clavier
        mInputManager->capture();
        mAnimationState->addTime(evt.timeSinceLastFrame);

        //mPlayerNode->yaw(mAngle,Node::TS_WORLD);
        //mPlayer->getSceneNode()->yaw(mAngle,Node::TS_WORLD);

        //Ici on doit swapper les coordonnees pour qu'elle soient les memes entre player et camera
        posY = (float)mDirection.y;
        posX = (float)mDirection.x;
        posZ = (float)mDirection.z;
        mDirection.y = posZ;
        mDirection.x = -posX;
        mDirection.z = posY;
        //mPlayerNode->translate(mDirection * evt.timeSinceLastFrame, Node::TS_LOCAL);
        //mPlayer->getSceneNode()->translate(mDirection * evt.timeSinceLastFrame, Node::TS_LOCAL);//n'a pas l'air de marcher avec les collision/gravité


        #ifdef COLLISIONS_GRAVITE_ACTIVEES
          mPlayer->setLinearVelocity(posX, posY, posZ);
          World::getSingleton().simulationStep(evt.timeSinceLastFrame);
        #else
          mPlayer->translate(mDirection * evt.timeSinceLastFrame);
        #endif

        //on remet les éléments de mDirection à leur place :
        mDirection.y = posY;
        mDirection.x = posX;
        mDirection.z = posZ;

        if (changement and iteration == 5) // si un déplacement a eu lieu pendant l'image précédente
        {
            bitBuffer buff;
            Vector3 pos;
            Quaternion dir; //oups comment ça marche cette merde?
            //pos = mPlayerNode->getPosition();
            pos=mPlayer->getSceneNode()->getPosition();
            //dir = mPlayerNode->getOrientation(); // renvoie un quaternion
            dir = mPlayer->getSceneNode()->getOrientation();
            posX = (float)pos.x; posY = (float)pos.y; posZ = (float)pos.z;

            //on remplit le buffer
            buff.writeFloat(posX); buff.writeFloat(posY); buff.writeFloat(posZ);
            buff.writeFloat(dirX); buff.writeFloat(dirY);
            buff.writeString(idClient);

            //on peut envoyer l'event d'update
            ogreManagerEvent.sendEvent(8,buff);

            changement = false; // on réinitialise changement

            iteration = 0;
        }


            pos=mPlayer->getSceneNode()->getPosition();
            posX = (float)pos.x; posY = (float)pos.y; posZ = (float)pos.z;

            //on verifie si on est proche d'un objet
            if (distance(55.0,392.0,120.0,posX,posY,posZ,50.0,50.0,50.0) && !lauchIntShow)
            // si la distance avec le point donne est assez petite et que la fenetre d'info n'est pas encore affichee, on l'affiche
            {
                lauchIntShow = true;
                showWindow(1,true);
            }
            else if (!distance(55.0,392.0,120.0,posX,posY,posZ,50.0,50.0,50.0) && lauchIntShow)
            // si winLaunchInter est montree et qu'on s'eloigne trop, on la cache
            {
                lauchIntShow = false;
                showWindow(1,false);
            }

          //on verifie les mise a jour du world
          event_data_t eventReceived;
          int i = 0;
          string id;
          structUser_t user;
          while(worldManagerEvent.changed())
          {
            //on check eventReceived.type et suivant le cas, on insere une nouvelle node au vecteur ou on en met une a jour
            eventReceived = worldManagerEvent.getEvent();
            switch (eventReceived.type)
            {
                case 3: //update
                    posX = eventReceived.data.readFloat(true);
                    posY = eventReceived.data.readFloat(true);
                    posZ = eventReceived.data.readFloat(true);
                    dirX = eventReceived.data.readFloat(true);
                    dirY = eventReceived.data.readFloat(true);
                    id = eventReceived.data.readString(true);
                    while (i < utilisateurs.size() && utilisateurs[i].id != id)
                    {
                        i++;
                    }
                    if (utilisateurs[i].id == id) //si l'user a updater existe bien
                    {
                        // on calcule les coordonnees de la translation a effectuer
                        posX -= (float)utilisateurs[i].node->getPosition().x;
                        posY -= (float)utilisateurs[i].node->getPosition().y;
                        posZ -= (float)utilisateurs[i].node->getPosition().z;
                        utilisateurs[i].node->translate(posX,posY,posZ, Node::TS_LOCAL);
                        //gerer la direction aussi :/
                    }
//                    else
//                    {
//                        user.id = id;
//
//                        SceneNode * userNode;
//                        userNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(id.data(), Vector3(posX,posY,posZ));
//                        Entity * userEntity = mSceneMgr->createEntity(id.data(), "man.mesh" );
//                        userEntity->setCastShadows(true);
//                        userNode->pitch(Degree(90));
//                        userNode->yaw(Degree(90));
//                        userNode->scale(Vector3(2,2,2));
//                        userNode->setFixedYawAxis(true, Vector3::UNIT_Z); // on redresse l'axe de la node egalement
//                        userNode->attachObject(userEntity); // on attache le modèle au noeud
//
//                        // XXX il faudra ajouter la direction XXX
//
//                        user.node = userNode;
//                        user.entite = userEntity;
//
//                        utilisateurs.push_back(user);
//
//                        // XXX on ecrit dans le chat qu'un nouvel user est connecte XXX
//                    }
                break;

                case 4: //add
                    id = eventReceived.data.readString(true);
                    user.id = id;

                    SceneNode * userNode;
                    userNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(id.data(), Vector3(-680,160,127));
                    Entity * userEntity = mSceneMgr->createEntity(id.data(), "man.mesh" );
                    userEntity->setCastShadows(true);
                    userNode->pitch(Degree(90));
                    userNode->yaw(Degree(90));
                    userNode->scale(Vector3(2,2,2));
                    userNode->setFixedYawAxis(true, Vector3::UNIT_Z); // on redresse l'axe de la node egalement
                    userNode->attachObject(userEntity); // on attache le modèle au noeud

                    user.node = userNode;
                    user.entite = userEntity;

                    utilisateurs.push_back(user);

                    // XXX on ecrit dans le chat qu'un nouvel user est connecte XXX
                break;

                case 5: //del
                    id = eventReceived.data.readString(true);

                    i = 0;
                    while (i < utilisateurs.size() && utilisateurs[i].id != id)
                    {
                        i++;
                    }
                    if (utilisateurs[i].id == id) //si l'user a updater existe bien
                    {
                        utilisateurs[i].entite->setVisible(false); // on rend l'entite invisible
                        free(utilisateurs[i].entite);
                        free(utilisateurs[i].node);
                        utilisateurs.erase(utilisateurs.begin()+i);
                        //supprimer l'entree du vecteur
                    }

                    // XXX on ecrit dans le chat qu'un user est deconnecte XXX
                break;

                default:
                    break;
            }
          }

        iteration++;

        return mContinue;
    }

    // MouseListener
    bool VortexFrameListener::mouseMoved(const OIS::MouseEvent &e)
    {
        mGUI->injectMouseMove(e);

    	  if(mMouse->getMouseState().buttonDown(OIS::MB_Right))
    	  {
    	    //on ne peut pas aller plus loin que 90 degré (camera a l'envers)
    	    if((mCamRotNode->getOrientation().getPitch().valueDegrees() < 90 && e.state.Y.rel > 0 )||
            (mCamRotNode->getOrientation().getPitch().valueDegrees() > -90 && e.state.Y.rel<0))
    	    {
            mCamRotNode->pitch(Degree(mRotate * e.state.Y.rel), Node::TS_LOCAL);//on tourne le camRotNode, mais pas le joueur
    	    }
            mPlayer->yaw(Degree(-mRotate * e.state.X.rel));//on tourne le joueur (et pas seulement le camRotNode)
            changement = true;
    	  }
        return true;
    }

    bool VortexFrameListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
        mGUI->injectMousePress(e, id);

    	if(e.state.buttonDown(OIS::MB_Right) && e.state.buttonDown(OIS::MB_Left))
    	{
    		mDirection.y = mMove;
    		if(mDirection.x==0)
    		{
    		  mAnimationState->setEnabled(false);
    	  	mAnimationState = mPlayer->getEntity()->getAnimationState("marcheAvant");
          mAnimationState->setLoop(true);
    		  mAnimationState->setEnabled(true);
    		}
    	}
      return true;
    }
    bool VortexFrameListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
        mGUI->injectMouseRelease(e, id);

        if(e.state.buttonDown(OIS::MB_Right) ||
            e.state.buttonDown(OIS::MB_Left))
        {
            mDirection.y = 0;
            if(mDirection.x==0)
                mAnimationState->setEnabled(false);
        }
        return true;
    }

    // KeyListener
    bool VortexFrameListener::keyPressed(const OIS::KeyEvent &e)
    {
        mGUI->injectKeyPress(e);

        bitBuffer buff;
        switch (e.key)
        {
            case OIS::KC_ESCAPE: // touche �chap, on sort du programme
                //on envoie l'event de deconnexion
                buff.writeString(idClient);
                worldManagerEvent.sendEvent(8,buff);
                mContinue = false;
                break;
            case OIS::KC_UP:
            case OIS::KC_W:
                mDirection.y = mMove; // on avance
                if(mDirection.x==0)
                {
                  mAnimationState->setEnabled(false);
                  mAnimationState = mPlayer->getEntity()->getAnimationState("marcheAvant");
                  mAnimationState->setLoop(true);
                  mAnimationState->setEnabled(true);
                }
                changement = true;
                break;

            case OIS::KC_DOWN:
            case OIS::KC_S:
                mDirection.y = -mMove/1.5; // on recule
                if(mDirection.x==0)
                {
                  mAnimationState->setEnabled(false);
                  mAnimationState = mPlayer->getEntity()->getAnimationState("marcheArriere");
                  mAnimationState->setLoop(true);
                  mAnimationState->setEnabled(true);
                }
                changement = true;
                break;

            case OIS::KC_LEFT:
            case OIS::KC_A:
                mDirection.x = -mMove/1.5; // on va � gauche
                mAnimationState->setEnabled(false);
                mAnimationState = mPlayer->getEntity()->getAnimationState("marcheGauche");
                mAnimationState->setLoop(true);
                mAnimationState->setEnabled(true);
                changement = true;
                break;

            case OIS::KC_RIGHT:
            case OIS::KC_D:
                mDirection.x = mMove/1.5; // on va � droite
                mAnimationState->setEnabled(false);
                mAnimationState = mPlayer->getEntity()->getAnimationState("marcheDroite");
                mAnimationState->setLoop(true);
                mAnimationState->setEnabled(true);
                changement = true;
                break;

            case OIS::KC_PGUP:
                mDirection.z = mMove/1.5; // on va en haut
                changement = true;
                break;

            case OIS::KC_PGDOWN:
                mDirection.z = -mMove/1.5; // on va en bas
                changement = true;
                break;

            default:
                break;
        }

        //return mContinue;
        return true;
    }


    bool VortexFrameListener::keyReleased(const OIS::KeyEvent &e)
    {
        mGUI->injectKeyRelease(e);

        switch (e.key) // ici on arr�te le mouvement lorsque la touche est relach�e
       {
       case OIS::KC_UP:
       case OIS::KC_W:
           mDirection.y = 0;
           if(mDirection.x==0)
           {
             mAnimationState->setEnabled(false);
           }
           break;

       case OIS::KC_DOWN:
       case OIS::KC_S:
           mDirection.y = 0;
           if(mDirection.x==0)
           {
             mAnimationState->setEnabled(false);
           }
           break;

       case OIS::KC_LEFT:
       case OIS::KC_A:
           mDirection.x = 0;
           mAngle=0;
           mAnimationState->setEnabled(false);//on désactive l'anim en cours (dans tous les cas)
           if(mDirection.y>0)
           {
             mAnimationState = mPlayer->getEntity()->getAnimationState("marcheAvant");
             mAnimationState->setLoop(true);
             mAnimationState->setEnabled(true);//on réactive l'anim dont on a besoin
           }
           else if(mDirection.y<0)
           {
             mAnimationState = mPlayer->getEntity()->getAnimationState("marcheArriere");
             mAnimationState->setLoop(true);
             mAnimationState->setEnabled(true);
           }
           break;

       case OIS::KC_RIGHT:
       case OIS::KC_D:
           mDirection.x = 0;
           mAngle=0;
           mAnimationState->setEnabled(false);
           if(mDirection.y>0)
           {
             mAnimationState = mPlayer->getEntity()->getAnimationState("marcheAvant");
             mAnimationState->setLoop(true);
             mAnimationState->setEnabled(true);//on réactive l'anim si besoin
           }
           else if(mDirection.y<0)
           {
             mAnimationState = mPlayer->getEntity()->getAnimationState("marcheArriere");
             mAnimationState->setLoop(true);
             mAnimationState->setEnabled(true);
           }
           break;

        case OIS::KC_PGUP:
        case OIS::KC_PGDOWN:
           mDirection.z = 0;
           break;

       default:
           break;
       } // switch
       return true;
    }
