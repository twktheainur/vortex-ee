#ifndef VORTEXFRAMELISTENER_H_INCLUDED
#define VORTEXFRAMELISTENER_H_INCLUDED

#include <unistd.h>
#include <Ogre.h>
#include "InputManager.h"
#include "globals.h"
#include "vector.h"
#include "PersonnagePhysique.h"


using namespace Ogre;

typedef struct structUser
{
  string id;
  SceneNode* node;
  Entity* entite;
} structUser_t;

class VortexFrameListener : public FrameListener, public OIS::MouseListener, public OIS::KeyListener
{
public:
    VortexFrameListener(RenderWindow* win, Camera* cam,PersonnagePhysique * player, SceneManager *sceneMgr);

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
    SceneNode *mCamRotNode; // le node autour duquel la caméra va tourner

    //SceneNode *mPlayerNode; // le node auquel le modele est attache
    //Entity *mPlayer;
    PersonnagePhysique* mPlayer;
    AnimationState *mAnimationState;

    bool mContinue;        // booleen definissant si on continue le rendu
    Vector3 mDirection;     // vecteur dfinissant le dplacement calcul
    Degree mAngle;  // Agle definissant la rotation calculee

    bool changement;  // booleen definissant si on a eu un mouvement pendant cette image

    vector<structUser_t> utilisateurs; // vecteur contenant les nodes des autres utilisateurs

    string idClient;
};

#endif // VORTEXFRAMELISTENER_H_INCLUDED
