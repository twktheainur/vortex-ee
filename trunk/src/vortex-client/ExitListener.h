#ifndef _EXITLISTENER_H
#define _EXITLISTENER_H

#include <Ogre.h>
#include <OIS/OIS.h>

using namespace Ogre;

class ExitListener : public FrameListener{
	public:
		ExitListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr,OIS::Keyboard *keyboard);
		bool frameStarted(const FrameEvent &evt);

    	// MouseListener
    	//bool mouseMoved(const OIS::MouseEvent &e) { return true; }
    	//bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) { return true; }
    	//bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) { return true; }

    	// KeyListener
		bool keyPressed(const OIS::KeyEvent &e);
		bool keyReleased(const OIS::KeyEvent &e);

	protected:
		Real mRotate;          // The rotate constant
		Real mMove;            // The movement constant

		SceneManager *mSceneMgr;   // The current SceneManager
		SceneNode *mCamNode;   // The SceneNode the camera is currently attached to

		OIS::Keyboard *mKeyboard;

		bool mContinue;        // Whether to continue rendering or not
		Vector3 mDirection;     // Value to move in the correct direction
};

#endif