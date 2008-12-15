#include "VortexFrameListener.h"

    VortexFrameListener::VortexFrameListener(RenderWindow* win, Camera* cam,Entity * player, SceneManager *sceneMgr)
        : ExampleFrameListener(win, cam, true, true)
    {
        // Populate the camera and scene manager containers
        mCamNode = cam->getParentSceneNode();
        mPlayer = player;
        mPlayerNode=player->getParentSceneNode();
        mSceneMgr = sceneMgr;

        mAnimationState = mPlayer->getAnimationState("marcheAvant");
        mAnimationState->setLoop(true);
        mAnimationState->setEnabled(false);

        mRotate = 0.20; // valeur � changer pour la sensibilit� de la souris
        mMove = 100; // vitesse de d�placement

        // bool�en d�finissant si on continue ou non le rendu des images
        mContinue = true;

        mMouse->setEventCallback(this);
        mKeyboard->setEventCallback(this);

        mDirection = Vector3::ZERO;
    }

    bool VortexFrameListener::frameStarted(const FrameEvent &evt)
    {
        // d'abord on capture les actions effectu�es � la souris et au clavier
        if(mMouse)
            mMouse->capture();
        if(mKeyboard)
            mKeyboard->capture();

        mAnimationState->addTime(evt.timeSinceLastFrame);
        mCamNode->translate(mDirection * evt.timeSinceLastFrame, Node::TS_LOCAL); // on met le node � jour � partir du vecteur calcul�

        int y = mDirection.y;
        int x = mDirection.x;
        mDirection.y = 0;
        mDirection.x = -x;
        mDirection.z = y;
        mPlayerNode->translate(mDirection * evt.timeSinceLastFrame, Node::TS_LOCAL);
        mDirection.y = y;
        mDirection.x = x;
        mDirection.z = 0;


        return mContinue;
    }

    // MouseListener
    bool VortexFrameListener::mouseMoved(const OIS::MouseEvent &e)
    {
        mCamNode->yaw(Degree(-0.5*mRotate * e.state.X.rel), Node::TS_WORLD);
        mCamNode->pitch(Degree(-mRotate * e.state.Y.rel), Node::TS_LOCAL);
        mPlayerNode->yaw(Degree(-3*mRotate*e.state.X.rel),Node::TS_WORLD);
        return true;
    }

    bool VortexFrameListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) { return false; }
    bool VortexFrameListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) { return false; }


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
                mAnimationState = mPlayer->getAnimationState("marcheAvant");
                mAnimationState->setLoop(true);
                mAnimationState->setEnabled(true);
                break;

            case OIS::KC_DOWN:
            case OIS::KC_S:
                mDirection.y = -mMove; // on recule
                mAnimationState = mPlayer->getAnimationState("marcheArriere");
                mAnimationState->setLoop(true);
                mAnimationState->setEnabled(true);
                break;

            case OIS::KC_LEFT:
            case OIS::KC_A:
                mDirection.x = -mMove; // on va � gauche
                mAnimationState = mPlayer->getAnimationState("marcheDroite");
                mAnimationState->setLoop(true);
                mAnimationState->setEnabled(true);
                break;

            case OIS::KC_RIGHT:
            case OIS::KC_D:
                mDirection.x = mMove; // on va � droite
                mAnimationState = mPlayer->getAnimationState("marcheGauche");
                mAnimationState->setLoop(true);
                mAnimationState->setEnabled(true);
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
           mAnimationState->setEnabled(false);
           break;

       case OIS::KC_RIGHT:
       case OIS::KC_D:
           mDirection.x = 0;
           mAnimationState->setEnabled(false);
           break;

       default:
           break;
       } // switch
       return true;
    }
