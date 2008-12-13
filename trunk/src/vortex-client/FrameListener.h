/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    ExampleFrameListener.h
Description: Defines an example frame listener which responds to frame events.
This frame listener just moves a specified camera around based on
keyboard and mouse movements.
Mouse:    Freelook
W or Up:  Forward
S or Down:Backward
A:        Step left
D:        Step right
             PgUp:     Move upwards
             PgDown:   Move downwards
             F:        Toggle frame rate stats on/off
			 R:        Render mode
             T:        Cycle texture filtering
                       Bilinear, Trilinear, Anisotropic(8)
             P:        Toggle on/off display of camera position / orientation
-----------------------------------------------------------------------------
*/


#ifndef _FRAMELISTENER_H
#define _FRAMELISTENER_H

#include <Ogre.h>
#include <OgreStringConverter.h>
#include <OgreException.h>

//#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

using namespace Ogre;

class FrameListener: public FrameListener, public WindowEventListener{

public:
	ExampleFrameListener(RenderWindow* win, Camera* cam, bool bufferedKeys = false, bool bufferedMouse = false, bool bufferedJoy = false );
	virtual ~ExampleFrameListener();

	virtual void windowResized(RenderWindow* rw);
	virtual void windowClosed(RenderWindow* rw);
	virtual bool processUnbufferedKeyInput(const FrameEvent& evt)
	virtual bool processUnbufferedMouseInput(const FrameEvent& evt)
	virtual void moveCamera()
	virtual void showDebugOverlay(bool show)
	bool frameRenderingQueued(const FrameEvent& evt)
	bool frameEnded(const FrameEvent& evt)

protected:

	virtual void updateStats(void);
	
	Camera* mCamera;

	Vector3 mTranslateVector;
	Real mCurrentSpeed;
	RenderWindow* mWindow;
	bool mStatsOn;

	std::string mDebugText;

	unsigned int mNumScreenShots;
	float mMoveScale;
	float mSpeedLimit;
	Degree mRotScale;
	// just to stop toggles flipping too fast
	Real mTimeUntilNextToggle ;
	Radian mRotX, mRotY;
	TextureFilterOptions mFiltering;
	int mAniso;

	int mSceneDetailIndex ;
	Real mMoveSpeed;
	Degree mRotateSpeed;
	Overlay* mDebugOverlay;

	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::JoyStick* mJoy;

};
#endif
