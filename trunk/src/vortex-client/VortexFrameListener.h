#ifndef VORTEXFRAMELISTENER_H_INCLUDED
#define VORTEXFRAMELISTENER_H_INCLUDED

#include <unistd.h>
#include <Ogre.h>
#include "InputManager.h"
#include "globals.h"
#include "vector.h"
#include "OgreReferenceAppLayer.h"//pour la gestion des collisions/gravité
#include "MyGUI.h"

//#define COLLISIONS_GRAVITE_ACTIVEES

using namespace Ogre;
using namespace OgreRefApp;

typedef struct structUser
{
  string id;
  SceneNode* node;
  Entity* entite;
} structUser_t;

class VortexFrameListener : public FrameListener, public OIS::MouseListener, public OIS::KeyListener
{
public:
    VortexFrameListener(RenderWindow* win, Camera* cam,PersonnagePhysique * player, SceneManager *sceneMgr, MyGUI::Gui * mGUI);

    inline void hideAccueil(MyGUI::WidgetPtr _sender) { showWindow(0,false); }

    inline void hideWindowImage(MyGUI::WidgetPtr _sender) { showWindow(3,false); }

    inline void hideWindowVideo(MyGUI::WidgetPtr _sender) { showWindow(2,false); }

    inline void hideWindowAudio(MyGUI::WidgetPtr _sender) { showWindow(4,false); }

	void notifyComboChatAccept(MyGUI::Widget * _sender);

    void addToChat(const Ogre::UTFString & _line);

    void showWindow(int window, bool show);
    // window est le numero de la fenetre qu'on montre ou cache suivant la valeur de show

    bool frameEnded(const FrameEvent &evt);

    // MouseListener
    bool mouseMoved(const OIS::MouseEvent &e);

    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);

    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

    // KeyListener
    bool keyPressed(const OIS::KeyEvent &e);

    bool keyReleased(const OIS::KeyEvent &e);

    inline bool distance(float x1, float y1, float z1, float x2, float y2, float z2, float distx, float disty, float distz)
    {
        float x, y, z;

        if (x1>x2) x = x1-x2;
        else x = x2-x1;

        if (y1>y2) y = y1-y2;
        else y = y2-y1;

        if (z1>z2) z = z1-z2;
        else z = z2-z1;

        return (x<distx && y<disty && z<distz);
    }

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
    int iteration; // variable définissant a quelle image on raffraichis la position du client

    vector<structUser_t> utilisateurs; // vecteur contenant les nodes des autres utilisateurs

    string idClient;

    Ogre::UTFString mStringUnknown;

    MyGUI::Gui * mGUI;

    MyGUI::VectorWidgetPtr winAccueil;

    MyGUI::VectorWidgetPtr winChat;
    MyGUI::ListPtr mChatHistorique;
    MyGUI::ComboBoxPtr mComboChat;

    MyGUI::VectorWidgetPtr winLaunchInter;

    MyGUI::VectorWidgetPtr winImage;
    MyGUI::VectorWidgetPtr winVideo;
    MyGUI::VectorWidgetPtr winAudio;

    bool lauchIntShow;
};

#endif // VORTEXFRAMELISTENER_H_INCLUDED
