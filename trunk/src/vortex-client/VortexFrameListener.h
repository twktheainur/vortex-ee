#ifndef VORTEXFRAMELISTENER_H_INCLUDED
#define VORTEXFRAMELISTENER_H_INCLUDED

#include "ExampleFrameListener.h"
#include <unistd.h>
using namespace Ogre;

class VortexFrameListener : public ExampleFrameListener, public OIS::MouseListener, public OIS::KeyListener
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
    Degree mRotate;          // constante de rotation
    Real mMove;            // constante de mouvement


    SceneManager *mSceneMgr;   // le scne manager courant
    SceneNode *mCamNode;   // le scenenode auquel notre camera et notre modle sont attachs

    SceneNode *mPlayerNode;
    Entity *mPlayer;
    AnimationState *mAnimationState;

    bool mContinue;        // boolen dfinissant si on continue le rendu
    Vector3 mDirection;     // vecteur dfinissant le dplacement calcul
    Degree mAngle;  // Agle definissant la rotation calculee
};

#endif // VORTEXFRAMELISTENER_H_INCLUDED
