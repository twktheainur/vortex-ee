#ifndef VORTEXFRAMELISTENER_H_INCLUDED
#define VORTEXFRAMELISTENER_H_INCLUDED

#include <unistd.h>
#include <Ogre.h>
#include "InputManager.h"


using namespace Ogre;

class VortexFrameListener : public FrameListener, public OIS::MouseListener, public OIS::KeyListener
{
public:
    VortexFrameListener(RenderWindow* win, Camera* cam,Entity * player, SceneManager *sceneMgr);

    bool frameStarted(const FrameEvent &evt);

    // MouseListener
    bool mouseMoved(const OIS::MouseEvent &e);

    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);

    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);


    // KeyListener
    bool keyPressed(const OIS::KeyEvent &e);


    bool keyReleased(const OIS::KeyEvent &e);

protected:

    InputManager * mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;

    Degree mRotate;          // constante de rotation
    Real mMove;            // constante de mouvement


    SceneManager *mSceneMgr;   // le scene manager courant
    SceneNode *mCamNode;   // le node auquel notre camera est attachee

    SceneNode *mPlayerNode; // le node auquel le modele est attache
    Entity *mPlayer;
    AnimationState *mAnimationState;

    bool mContinue;        // booleen definissant si on continue le rendu
    Vector3 mDirection;     // vecteur dfinissant le dplacement calcul
    Degree mAngle;  // Agle definissant la rotation calculee

    bool changement;  // booleen definissant si on a eu un mouvement pendant cette image
};

#endif // VORTEXFRAMELISTENER_H_INCLUDED
