#include "exitlistener.h"

#ifdef _EXITLISTENER_H
#include <Ogre.h>
#include <OIS/OIS.h>

    ExitListener::ExitListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr,OIS::Keyboard *keyboard)
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


    bool ExitListener::frameStarted(const FrameEvent &evt)
    {
      //  if(mMouse)
        //    mMouse->capture();
        if(mKeyboard)
            mKeyboard->capture();
        mCamNode->translate(mDirection * evt.timeSinceLastFrame, Node::TS_LOCAL);
        return mContinue;
    }

    // KeyListener
    bool ExitListener::keyPressed(const OIS::KeyEvent &e)
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
    
    bool ExitListener::keyReleased(const OIS::KeyEvent &e)
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
#endif
