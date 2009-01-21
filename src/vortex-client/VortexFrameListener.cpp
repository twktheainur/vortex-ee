#include "VortexFrameListener.h"
#include "../common/bitBuffer.h"

    VortexFrameListener::VortexFrameListener(RenderWindow* win, Camera* cam,Entity * player, SceneManager *sceneMgr)
    {
      mInputManager = InputManager::getSingletonPtr();
      mInputManager->initialise( win );
      mInputManager->addKeyListener( this, "VortexFrameListener" );
      mInputManager->addMouseListener( this, "VortexFrameListener" );
      mMouse = mInputManager->getMouse();
      mKeyboard = mInputManager->getKeyboard();


        // Populate the camera and scene manager containers
        mCamNode = cam->getParentSceneNode();
        mCamRotNode = mCamNode->getParentSceneNode();
        mPlayer = player;
        mPlayerNode=player->getParentSceneNode();
        mSceneMgr = sceneMgr;

        mAnimationState = mPlayer->getAnimationState("marcheAvant");
        mAnimationState->setLoop(true);
        mAnimationState->setEnabled(false);

        mRotate = 0.2; // valeur � changer pour la sensibilit� de la souris
        mMove = 100; // vitesse de d�placement

        // bool�en d�finissant si on continue ou non le rendu des images
        mContinue = true;

        mDirection = Vector3::ZERO;

        changement = false; // on initialise le booleen de changement
    }

    bool VortexFrameListener::frameStarted(const FrameEvent &evt)
    {
        // on declare les variables de position et direction qui nous servirons plusieurs fois par la suite
        float posX;
        float posY;
        float posZ;
        float dirX = 0;
        float dirY = 0;

        // d'abord on capture les actions effectu�es � la souris et au clavier
        mInputManager->capture();
        mAnimationState->addTime(evt.timeSinceLastFrame);

        mPlayerNode->yaw(mAngle,Node::TS_WORLD);

        //Ici on doit swapper les coordonnees pour qu'elle soient les memes entre player et camera
        posY = (float)mDirection.y;
        posX = (float)mDirection.x;
        posZ = (float)mDirection.z;
        mDirection.y = posZ;
        mDirection.x = -posX;
        mDirection.z = posY;
        mPlayerNode->translate(mDirection * evt.timeSinceLastFrame, Node::TS_LOCAL);
        mDirection.y = posY;
        mDirection.x = posX;
        mDirection.z = posZ;

        if (changement) // si un déplacement a eu lieu pendant l'image précédente
        {
            bitBuffer buff;
            Vector3 pos;
            Quaternion dir; //oups comment ça marche cette merde?
            pos = mPlayerNode->getPosition();
            dir = mPlayerNode->getOrientation(); // renvoie un quaternion
            posX = (float)pos.x; posY = (float)pos.y; posZ = (float)pos.z;

            // on ajoute l'id du client au buffer avant
            buff.writeFloat(posX); buff.writeFloat(posY); buff.writeFloat(posZ);
            buff.writeFloat(dirX); buff.writeFloat(dirY);

            //on peut envoyer l'event d'update
            worldManagerEvent.sendEvent(8,buff);

            changement = false; // on réinitialise changement
        }


        // ici on doit aussi vérifier si il y a des mise à jour du world
        event_data_t eventReceived;

          int i = 0;

          //apres on check eventReceived.type et suivant le cas, on insere une nouvelle node au vecteur ou on en met une a jour
          while(worldManagerEvent.changed())
          {
            eventReceived = worldManagerEvent.getEvent();
            switch (eventReceived.type)
            {
                case 3: //update
                    //char* id = eventReceived.data.readChar(true);
                    posX = eventReceived.data.readFloat(true); posY = eventReceived.data.readFloat(true); posZ = eventReceived.data.readFloat(true);
                    dirX = eventReceived.data.readFloat(true); dirY = eventReceived.data.readFloat(true);
                    while (i < utilisateurs.size() and utilisateurs[i].id != "plop")
                    {
                        i++;
                    }
                    if (utilisateurs[i].id == "plop") //si l'user a updater existe bien
                    {
                        // on calcule les coordonnees de la translation a effectuer
                        posX -= (float)utilisateurs[i].node->getPosition().x;
                        posY -= (float)utilisateurs[i].node->getPosition().y;
                        posZ -= (float)utilisateurs[i].node->getPosition().z;
                        utilisateurs[i].node->translate(posX,posY,posZ, Node::TS_LOCAL);
                        //gerer la direction aussi :/
                    }
                    else
                    {
                        //il faut creer l'utilisateur a la position donnee
                    }
                break;

                case 4: //add
                break;

                case 5: //del
                break;

                default:
                    break;
            }
          }

        return mContinue;
    }

    // MouseListener
    bool VortexFrameListener::mouseMoved(const OIS::MouseEvent &e)
    {
    	  if(mMouse->getMouseState().buttonDown(OIS::MB_Right))
    	  {
    	    //on ne peut pas aller plus loin que 90 degré (camera a l'envers)
    	    if((mCamRotNode->getOrientation().getPitch().valueDegrees() < 90 && e.state.Y.rel > 0 )||
            (mCamRotNode->getOrientation().getPitch().valueDegrees() > -90 && e.state.Y.rel<0))
    	    {
            mCamRotNode->pitch(Degree(mRotate * e.state.Y.rel), Node::TS_LOCAL);//on tourne le camRotNode, mais pas le joueur
    	    }
            mPlayerNode->yaw(Degree(-mRotate * e.state.X.rel), Node::TS_WORLD);//on tourne le joueur (et pas seulement le camRotNode)
            changement = true;
    	  }
        return true;
    }

    bool VortexFrameListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
    	if(e.state.buttonDown(OIS::MB_Right) && e.state.buttonDown(OIS::MB_Left))
    	{
    		mDirection.y = mMove;
    		if(mDirection.x==0)
    		{
    		  mAnimationState->setEnabled(false);
    	  	mAnimationState = mPlayer->getAnimationState("marcheAvant");
          mAnimationState->setLoop(true);
    		  mAnimationState->setEnabled(true);
    		}
    	}
      return true;
    }
    bool VortexFrameListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
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
        switch (e.key)
        {
            case OIS::KC_ESCAPE: // touche �chap, on sort du programme
                mContinue = false;
                break;
            case OIS::KC_UP:
            case OIS::KC_W:
                mDirection.y = mMove; // on avance
                if(mDirection.x==0)
                {
                  mAnimationState->setEnabled(false);
                  mAnimationState = mPlayer->getAnimationState("marcheAvant");
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
                  mAnimationState = mPlayer->getAnimationState("marcheArriere");
                  mAnimationState->setLoop(true);
                  mAnimationState->setEnabled(true);
                }
                changement = true;
                break;

            case OIS::KC_LEFT:
            case OIS::KC_A:
                mDirection.x = -mMove/1.5; // on va � gauche
                mAnimationState->setEnabled(false);
                mAnimationState = mPlayer->getAnimationState("marcheGauche");
                mAnimationState->setLoop(true);
                mAnimationState->setEnabled(true);
                changement = true;
                break;

            case OIS::KC_RIGHT:
            case OIS::KC_D:
                mDirection.x = mMove/1.5; // on va � droite
                mAnimationState->setEnabled(false);
                mAnimationState = mPlayer->getAnimationState("marcheDroite");
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
             mAnimationState = mPlayer->getAnimationState("marcheAvant");
             mAnimationState->setLoop(true);
             mAnimationState->setEnabled(true);//on réactive l'anim dont on a besoin
           }
           else if(mDirection.y<0)
           {
             mAnimationState = mPlayer->getAnimationState("marcheArriere");
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
             mAnimationState = mPlayer->getAnimationState("marcheAvant");
             mAnimationState->setLoop(true);
             mAnimationState->setEnabled(true);//on réactive l'anim si besoin
           }
           else if(mDirection.y<0)
           {
             mAnimationState = mPlayer->getAnimationState("marcheArriere");
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
