#include "VortexFrameListener.h"

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
        mPlayer = player;
        mPlayerNode=player->getParentSceneNode();
        mSceneMgr = sceneMgr;

        mAnimationState = mPlayer->getAnimationState("marcheAvant");
        mAnimationState->setLoop(true);
        mAnimationState->setEnabled(false);

        mRotate = 0.3; // valeur � changer pour la sensibilit� de la souris
        mMove = 100; // vitesse de d�placement

        // bool�en d�finissant si on continue ou non le rendu des images
        mContinue = true;

        mDirection = Vector3::ZERO;

        changement = false; // on initialise le booleen de changement
    }

    bool VortexFrameListener::frameStarted(const FrameEvent &evt)
    {
        // d'abord on capture les actions effectu�es � la souris et au clavier
        mInputManager->capture();
        mAnimationState->addTime(evt.timeSinceLastFrame);

        mPlayerNode->yaw(mAngle,Node::TS_WORLD);

        //Ici on doit swapper les coordonnees pour qu'elle soient les memes entre player et camera
        int y = mDirection.y;
        int x = mDirection.x;
        mDirection.y = 0;
        mDirection.x = -x;
        mDirection.z = y;
        mPlayerNode->translate(mDirection * evt.timeSinceLastFrame, Node::TS_LOCAL);
        mDirection.y = y;
        mDirection.x = x;
        mDirection.z = 0;

        if (changement) // si un déplacement a eu lieu pendant l'image précédente
        {
            // Event::event.ogre.update
            changement = false; // on réinitialise changement
        }


        // ici on doit aussi vérifier si il y a des mise à jour du world

        return mContinue;
    }

    // MouseListener
    bool VortexFrameListener::mouseMoved(const OIS::MouseEvent &e)
    {
    	  if(mMouse->getMouseState().buttonDown(OIS::MB_Right))
    	  {
            mCamNode->pitch(Degree(-mRotate * e.state.Y.rel), Node::TS_LOCAL);
            mPlayerNode->yaw(Degree(-mRotate * e.state.X.rel), Node::TS_WORLD);
            changement = true;
    	  }
        return true;
    }

    bool VortexFrameListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
    	if(e.state.buttonDown(OIS::MB_Right) && e.state.buttonDown(OIS::MB_Left))
    	{
    		mDirection.y = mMove;
    		mAnimationState = mPlayer->getAnimationState("marcheAvant");
    		                mAnimationState->setLoop(true);
    		                mAnimationState->setEnabled(true);
    	}
      return true;
    }
    bool VortexFrameListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
      if(e.state.buttonDown(OIS::MB_Right) ||
      	 e.state.buttonDown(OIS::MB_Left))
			{
        mDirection.y = 0;
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
                mAnimationState->setEnabled(false);
                mAnimationState = mPlayer->getAnimationState("marcheAvant");
                mAnimationState->setLoop(true);
                mAnimationState->setEnabled(true);
                changement = true;
                break;

            case OIS::KC_DOWN:
            case OIS::KC_S:
                mDirection.y = -mMove/1.5; // on recule
                mAnimationState->setEnabled(false);
                mAnimationState = mPlayer->getAnimationState("marcheArriere");
                mAnimationState->setLoop(true);
                mAnimationState->setEnabled(true);
                changement = true;
                break;

            case OIS::KC_LEFT:
            case OIS::KC_A:
                mDirection.x = -mMove/1.5; // on va � gauche
                // mAngle = mRotate;
                mAnimationState->setEnabled(false);
                mAnimationState = mPlayer->getAnimationState("marcheGauche");
                mAnimationState->setLoop(true);
                mAnimationState->setEnabled(true);
                changement = true;
                break;

            case OIS::KC_RIGHT:
            case OIS::KC_D:
                mDirection.x = mMove/1.5; // on va � droite
            	// mAngle = -mRotate;
                mAnimationState->setEnabled(false);
                mAnimationState = mPlayer->getAnimationState("marcheDroite");
                mAnimationState->setLoop(true);
                mAnimationState->setEnabled(true);
                changement = true;
                break;
            default:
                break;
        }

        return mContinue;
    }


    bool VortexFrameListener::keyReleased(const OIS::KeyEvent &e)
    {
        switch (e.key) // ici on arr�te le mouvement lorsque la touche est relach�e
       {
       case OIS::KC_UP:
       case OIS::KC_W:
           mDirection.y = 0;
           mAnimationState->setEnabled(false);
           break;

       case OIS::KC_DOWN:
       case OIS::KC_S:
           mDirection.y = 0;
           mAnimationState->setEnabled(false);
           break;

       case OIS::KC_LEFT:
       case OIS::KC_A:
           mDirection.x = 0;
           mAngle=0;
           mAnimationState->setEnabled(false);
           break;

       case OIS::KC_RIGHT:
       case OIS::KC_D:
           mDirection.x = 0;
           mAngle=0;
           mAnimationState->setEnabled(false);
           break;

       default:
           break;
       } // switch
       return true;
    }
