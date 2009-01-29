#include "VortexFrameListener.h"
#include "../common/bitBuffer.h"

VortexFrameListener::VortexFrameListener(RenderWindow* win, Camera* cam,PersonnagePhysique * player, SceneManager *sceneMgr, MyGUI::Gui * mGUI)
{
    mInputManager = InputManager::getSingletonPtr();
    mInputManager->initialise( win );
    mInputManager->addKeyListener( this, "VortexFrameListener" );
    mInputManager->addMouseListener( this, "VortexFrameListener" );
    mMouse = mInputManager->getMouse();
    mKeyboard = mInputManager->getKeyboard();

    this->mGUI = mGUI;

    //fenetre de chat
    winChat = MyGUI::LayoutManager::getInstance().load("chat.layout");
    mChatHistorique = mGUI->findWidget<MyGUI::List>("chatHistorique");
    mComboChat = mGUI->findWidget<MyGUI::ComboBox>("comboChat");

    mComboChat->eventComboAccept = MyGUI::newDelegate(this, &VortexFrameListener::notifyComboChatAccept);

    //fenetre qui apparait lorsqu'on est proche d'un objet interactif
    winLaunchInter = MyGUI::LayoutManager::getInstance().load("launchInterface.layout");
    showWindow(1,false);

    //interface documents
    winImage = MyGUI::LayoutManager::getInstance().load("winImage.layout");
    // set callback
    MyGUI::ButtonPtr buttonImageFermer = mGUI->findWidget<MyGUI::Button>("buttonImageFermer");
    buttonImageFermer->eventMouseButtonClick = MyGUI::newDelegate(this, &VortexFrameListener::closeWindowImage);
    // set callback2
    MyGUI::ButtonPtr buttonImageCacher = mGUI->findWidget<MyGUI::Button>("buttonImageCacher");
    buttonImageCacher->eventMouseButtonClick = MyGUI::newDelegate(this, &VortexFrameListener::hideWindowImage);

    winVideo = MyGUI::LayoutManager::getInstance().load("winVideo.layout");
    // set callback
    MyGUI::ButtonPtr buttonVideoFermer = mGUI->findWidget<MyGUI::Button>("buttonVideoFermer");
    buttonVideoFermer->eventMouseButtonClick = MyGUI::newDelegate(this, &VortexFrameListener::closeWindowVideo);
    // set callback2
    MyGUI::ButtonPtr buttonVideoCacher = mGUI->findWidget<MyGUI::Button>("buttonVideoCacher");
    buttonVideoCacher->eventMouseButtonClick = MyGUI::newDelegate(this, &VortexFrameListener::hideWindowVideo);

    winAudio = MyGUI::LayoutManager::getInstance().load("winAudio.layout");
    // set callback
    MyGUI::ButtonPtr buttonAudioFermer = mGUI->findWidget<MyGUI::Button>("buttonAudioFermer");
    buttonAudioFermer->eventMouseButtonClick = MyGUI::newDelegate(this, &VortexFrameListener::closeWindowAudio);
    // set callback2
    MyGUI::ButtonPtr buttonAudioCacher = mGUI->findWidget<MyGUI::Button>("buttonAudioCacher");
    buttonAudioCacher->eventMouseButtonClick = MyGUI::newDelegate(this, &VortexFrameListener::hideWindowAudio);

    //boutons d'affichage des interfaces
    buttonsAfficher = MyGUI::LayoutManager::getInstance().load("boutonAfficher.layout");
    buttonImageAfficher = mGUI->findWidget<MyGUI::Button>("buttonImageAfficher");
    buttonImageAfficher->eventMouseButtonClick = MyGUI::newDelegate(this, &VortexFrameListener::showWindowImage);
    buttonAudioAfficher = mGUI->findWidget<MyGUI::Button>("buttonAudioAfficher");
    buttonAudioAfficher->eventMouseButtonClick = MyGUI::newDelegate(this, &VortexFrameListener::showWindowAudio);
    buttonVideoAfficher = mGUI->findWidget<MyGUI::Button>("buttonVideoAfficher");
    buttonVideoAfficher->eventMouseButtonClick = MyGUI::newDelegate(this, &VortexFrameListener::showWindowVideo);


    // Populate the camera and scene manager containers
    mCamNode = cam->getParentSceneNode();
    mCamRotNode = mCamNode->getParentSceneNode();
    mPlayer = player;
    //mPlayerNode=player->getParentSceneNode();
    mSceneMgr = sceneMgr;

    mAnimationState = mPlayer->getEntity()->getAnimationState("marcheAvant");
    mAnimationState->setLoop(true);
    mAnimationState->setEnabled(false);

    mRotate = 0.2; // valeur � changer pour la sensibilit� de la souris
    mMove = 100; // vitesse de d�placement

    // bool�en d�finissant si on continue ou non le rendu des images
    mContinue = true;

    mDirection = Vector3::ZERO;

    changement = false; // on initialise le booleen de changement
    iteration = 5;

    idClient = "";

    launchIntShow = false;

    messageBienvenue = false;
}

void VortexFrameListener::notifyComboChatAccept(MyGUI::Widget * _sender)
{
    const Ogre::UTFString & message = _sender->getCaption();
    if (message == "") return;

    addToChat( "#00003B" + idClient + ": #006B85" + message);

        bitBuffer buff;
        buff.writeString(idClient);
        buff.writeString("all");
        buff.writeString(message);

        //on peut envoyer l'event d'update
        ogreManagerEvent.sendEvent(6,buff);

    _sender->setCaption("");
}

void VortexFrameListener::addToChat(const Ogre::UTFString & _line)
{
    mChatHistorique->addItem(_line);
    mChatHistorique->beginToEnd();
}

void VortexFrameListener::showWindow(int window, bool show)
{
    switch (window)
    {
    case 0: //winAccueil
        if (show) winAccueil[0]->show();
        else winAccueil[0]->hide();
        break;

    case 1: //winLaunchInter
        if (show) winLaunchInter[0]->show();
        else winLaunchInter[0]->hide();
        break;

    case 2: //winVideo
        if (show) { winVideo[0]->show(); buttonVideoAfficher->hide(); }
        else winVideo[0]->hide();
        break;

    case 3: //winImage
        if (show) { winImage[0]->show(); buttonImageAfficher->hide(); }
        else winImage[0]->hide();
        break;

    case 4: //winAudio
        if (show) { winAudio[0]->show(); buttonAudioAfficher->hide(); }
        else winAudio[0]->hide();
        break;

    default:
        break;
    }
}

bool VortexFrameListener::frameEnded(const FrameEvent &evt)
{
    if (idClient == "")
        idClient = idClientGlobal;
    else if (idClient != "" && !messageBienvenue)
    {
        addToChat("#000088Bienvenue dans Vortex #00003B" + idClient + "#000088 !");

        //fenetre d'accueil
        winAccueil = MyGUI::LayoutManager::getInstance().load("accueil.layout");
        // set callback
        MyGUI::ButtonPtr buttonAccueilOK = mGUI->findWidget<MyGUI::Button>("buttonAccueilOK");
        buttonAccueilOK->eventMouseButtonClick = MyGUI::newDelegate(this, &VortexFrameListener::hideAccueil);

        messageBienvenue = true;
    }

    mGUI->injectFrameEntered(evt.timeSinceLastFrame);

    // on declare les variables de position et direction qui nous servirons plusieurs fois par la suite
    Vector3 pos;
    float posX;
    float posY;
    float posZ;
    float dirX = 0.0;
    float dirY = 0.0;
    int anim;

    // d'abord on capture les actions effectu�es � la souris et au clavier
    mInputManager->capture();
    mAnimationState->addTime(evt.timeSinceLastFrame);

    //mPlayerNode->yaw(mAngle,Node::TS_WORLD);
    //mPlayer->getSceneNode()->yaw(mAngle,Node::TS_WORLD);

    //Ici on doit swapper les coordonnees pour qu'elle soient les memes entre player et camera
    posY = (float)mDirection.y;
    posX = (float)mDirection.x;
    posZ = (float)mDirection.z;
    mDirection.y = posZ;
    mDirection.x = -posX;
    mDirection.z = posY;
    //mPlayerNode->translate(mDirection * evt.timeSinceLastFrame, Node::TS_LOCAL);
    //mPlayer->getSceneNode()->translate(mDirection * evt.timeSinceLastFrame, Node::TS_LOCAL);
    mPlayer->deplacement(mDirection, evt.timeSinceLastFrame);

    //on remet les éléments de mDirection à leur place :
    mDirection.y = posY;
    mDirection.x = posX;
    mDirection.z = posZ;

    if (iteration >= 5) // si un déplacement a eu lieu pendant l'image précédente
    {
        bitBuffer buff;
        Vector3 pos;
        Quaternion dir; //oups comment ça marche cette merde?
        //pos = mPlayerNode->getPosition();
        pos=mPlayer->getSceneNode()->getPosition();
        //dir = mPlayerNode->getOrientation(); // renvoie un quaternion
        dir = mPlayer->getSceneNode()->getOrientation();
        posX = (float)pos.x;
        posY = (float)pos.y;
        posZ = (float)pos.z;

        if (mAnimationState->getAnimationName() == "MarcheAvant") anim = 1;
        else if (mAnimationState->getAnimationName() == "MarcheArriere") anim = 2;
        else if (mAnimationState->getAnimationName() == "MarcheGauche") anim = 3;
        else if (mAnimationState->getAnimationName() == "MarcheDroite") anim = 4;
        else anim = 0;

        //on remplit le buffer
        buff.writeFloat(posX);
        buff.writeFloat(posY);
        buff.writeFloat(posZ);
        buff.writeFloat(dirX);
        buff.writeFloat(dirY);
        buff.writeString(idClient);
        buff.writeInt(anim);

        //on peut envoyer l'event d'update
        ogreManagerEvent.sendEvent(8,buff);

        changement = false; // on réinitialise changement

        iteration = 0;
    }


    pos=mPlayer->getSceneNode()->getPosition();
    posX = (float)pos.x;
    posY = (float)pos.y;
    posZ = (float)pos.z;

    //on verifie si on est proche d'un objet
    if ((distance(55.0,392.0,120.0,posX,posY,posZ,80.0,80.0,80.0) || distance(144.0,384.0,72.0,posX,posY,posZ,80.0,80.0,80.0) ||
            distance(-627.0,560.0,168.0,posX,posY,posZ,80.0,80.0,80.0) || distance(-752.0,472.0,168.0,posX,posY,posZ,80.0,80.0,80.0) ||
            distance(736.0,-240.0,96.0,posX,posY,posZ,80.0,80.0,80.0) || distance(816.0,-160.0,96.0,posX,posY,posZ,80.0,80.0,80.0) ||
            distance(224.0,-232.0,104.0,posX,posY,posZ,80.0,80.0,80.0)) && !launchIntShow)
        // TV(2), bib1(2), bib2(2), audio(1)
        // si la distance avec le point donne est assez petite et que la fenetre d'info n'est pas encore affichee, on l'affiche
    {
        launchIntShow = true;
        showWindow(1,true);
    }
    else if ((!distance(55.0,392.0,120.0,posX,posY,posZ,80.0,80.0,80.0) && !distance(144.0,384.0,72.0,posX,posY,posZ,80.0,80.0,80.0) &&
              !distance(-627.0,560.0,168.0,posX,posY,posZ,80.0,80.0,80.0) && !distance(-752.0,472.0,168.0,posX,posY,posZ,80.0,80.0,80.0) &&
              !distance(736.0,-240.0,96.0,posX,posY,posZ,80.0,80.0,80.0) && !distance(816.0,-160.0,96.0,posX,posY,posZ,80.0,80.0,80.0) &&
              !distance(224.0,-232.0,104.0,posX,posY,posZ,80.0,80.0,80.0)) && launchIntShow)
        // si winLaunchInter est montree et qu'on s'eloigne trop, on la cache
    {
        launchIntShow = false;
        showWindow(1,false);
    }

    //on verifie les mise a jour du world
    event_data_t eventReceived;
    int i = 0;
    string id;
    structUser_t user;

    string envoyeur;
    string receveur;
    string message;

    while (worldManagerEvent.changed())
    {
        //on check eventReceived.type et suivant le cas, on insere une nouvelle node au vecteur ou on en met une a jour
        eventReceived = worldManagerEvent.getEvent();
        switch (eventReceived.type)
        {
        case 3: //update
            posX = eventReceived.data.readFloat(true);
            posY = eventReceived.data.readFloat(true);
            posZ = eventReceived.data.readFloat(true);
            dirX = eventReceived.data.readFloat(true);
            dirY = eventReceived.data.readFloat(true);
            id = eventReceived.data.readString(true);
            anim = eventReceived.data.readInt(true);
            while (i < utilisateurs.size() && utilisateurs[i].id != id)
            {
                i++;
            }
            if (utilisateurs[i].id == id) //si l'user a updater existe bien
            {
                // on calcule les coordonnees de la translation a effectuer
                posX -= (float)utilisateurs[i].node->getPosition().x;
                posY -= (float)utilisateurs[i].node->getPosition().y;
                posZ -= (float)utilisateurs[i].node->getPosition().z;
                utilisateurs[i].node->translate(posX,posY,posZ, Node::TS_LOCAL);
                //gerer la direction aussi :/
            }

            //on gere l'animation
            if (anim == 0)
                utilisateurs[i].anim->setEnabled(false);
            else if (anim == 1)
            {
                utilisateurs[i].anim->setEnabled(false);
                utilisateurs[i].anim = utilisateurs[i].entite->getAnimationState("marcheAvant");
                utilisateurs[i].anim->setLoop(true);
                utilisateurs[i].anim->setEnabled(true);
            }
            else if (anim == 2)
            {
                utilisateurs[i].anim->setEnabled(false);
                utilisateurs[i].anim = utilisateurs[i].entite->getAnimationState("marcheArriere");
                utilisateurs[i].anim->setLoop(true);
                utilisateurs[i].anim->setEnabled(true);
            }
            else if (anim == 3)
            {
                utilisateurs[i].anim->setEnabled(false);
                utilisateurs[i].anim = utilisateurs[i].entite->getAnimationState("marcheGauche");
                utilisateurs[i].anim->setLoop(true);
                utilisateurs[i].anim->setEnabled(true);
            }
            else if (anim == 4)
            {
                utilisateurs[i].anim->setEnabled(false);
                utilisateurs[i].anim = utilisateurs[i].entite->getAnimationState("marcheDroite");
                utilisateurs[i].anim->setLoop(true);
                utilisateurs[i].anim->setEnabled(true);
            }

        break;

        case 4: //add
            id = eventReceived.data.readString(true);
            user.id = id;

            SceneNode * userNode;
            AnimationState * anim;
            Entity * userEntity;
            userNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(id.data(), Vector3(-680,160,127));
            userEntity = mSceneMgr->createEntity(id.data(), "man.mesh" );
            userEntity->setCastShadows(true);
            userNode->pitch(Degree(90));
            userNode->yaw(Degree(90));
            userNode->scale(Vector3(2,2,2));
            userNode->setFixedYawAxis(true, Vector3::UNIT_Z); // on redresse l'axe de la node egalement
            userNode->attachObject(userEntity); // on attache le modèle au noeud

            anim = userEntity->getAnimationState("marcheAvant");
            anim->setLoop(true);
            anim->setEnabled(false);

            user.node = userNode;
            user.entite = userEntity;
            user.anim = anim;

            utilisateurs.push_back(user);

            addToChat("#FF8000" + id + "vient de se connecter.");
        break;

        case 5: //del
            id = eventReceived.data.readString(true);

            i = 0;
            while (i < utilisateurs.size() && utilisateurs[i].id != id)
            {
                i++;
            }
            if (utilisateurs[i].id == id) //si l'user a updater existe bien
            {
                utilisateurs[i].entite->setVisible(false); // on rend l'entite invisible
                free(utilisateurs[i].entite);
                free(utilisateurs[i].node);
                free(utilisateurs[i].anim);
                utilisateurs.erase(utilisateurs.begin()+i);
                //supprimer l'entree du vecteur
            }

            addToChat("#FF8000" + id + "vient de se deconnecter.");
        break;

        case 6: // message de chat
            envoyeur = eventReceived.data.readString(true);
            receveur = eventReceived.data.readString(true);
            message = eventReceived.data.readString(true);

            addToChat("#351E3C" + envoyeur + ": #6C4F9D" + message);
        break;

        default:
            break;
        }
    }

    iteration++;

    if (mContinue) mContinue = continuer;
    return mContinue;
}

// MouseListener
bool VortexFrameListener::mouseMoved(const OIS::MouseEvent &e)
{
    mGUI->injectMouseMove(e);

    if (idClient != "") {
    if (mMouse->getMouseState().buttonDown(OIS::MB_Right))
    {
        changement = true;
        //on ne peut pas aller plus loin que 90 degré (camera a l'envers)
        if ((mCamRotNode->getOrientation().getPitch().valueDegrees() < 90 && e.state.Y.rel > 0 )||
                (mCamRotNode->getOrientation().getPitch().valueDegrees() > -90 && e.state.Y.rel<0))
        {
            mCamRotNode->pitch(Degree(mRotate * e.state.Y.rel), Node::TS_LOCAL);//on tourne le camRotNode, mais pas le joueur
        }
        mPlayer->getSceneNode()->yaw(Degree(-mRotate * e.state.X.rel));//on tourne le joueur (et pas seulement le camRotNode)
    }
    }
    return true;
}

bool VortexFrameListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    mGUI->injectMousePress(e, id);

    if (idClient != "") {
    if (e.state.buttonDown(OIS::MB_Right) && e.state.buttonDown(OIS::MB_Left))
    {
        mDirection.y = mMove;
        if (mDirection.x==0)
        {
            changement = true;
            mAnimationState->setEnabled(false);
            mAnimationState = mPlayer->getEntity()->getAnimationState("marcheAvant");
            mAnimationState->setEnabled(true);
        }
    }
    }
    return true;
}
bool VortexFrameListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    mGUI->injectMouseRelease(e, id);

    if (idClient != "") {
    if (e.state.buttonDown(OIS::MB_Right) ||
            e.state.buttonDown(OIS::MB_Left))
    {
        mDirection.y = 0;
        if (mDirection.x==0)
            mAnimationState->setEnabled(false);
    }
    }
    return true;
}

// KeyListener
bool VortexFrameListener::keyPressed(const OIS::KeyEvent &e)
{
    mGUI->injectKeyPress(e);

    Vector3 pos;
    float posX, posY, posZ;

    bitBuffer buff;
    switch (e.key)
    {
    case OIS::KC_ESCAPE: // touche echap, on sort du programme
        //on envoie l'event de deconnexion
        buff.writeString(idClient);
        worldManagerEvent.sendEvent(8,buff);
        mContinue = false;
        break;
    case OIS::KC_UP:
    case OIS::KC_W:
    if (idClient != "") {
        mDirection.y = mMove; // on avance
        if (mDirection.x==0)
        {
            mAnimationState->setEnabled(false);
            mAnimationState = mPlayer->getEntity()->getAnimationState("marcheAvant");
            mAnimationState->setLoop(true);
            mAnimationState->setEnabled(true);
        }
        changement = true;
    }
    break;

    case OIS::KC_DOWN:
    case OIS::KC_S:
    if (idClient != "") {
        mDirection.y = -mMove/1.5; // on recule
        if (mDirection.x==0)
        {
            mAnimationState->setEnabled(false);
            mAnimationState = mPlayer->getEntity()->getAnimationState("marcheArriere");
            mAnimationState->setLoop(true);
            mAnimationState->setEnabled(true);
        }
        changement = true;
    }
    break;

    case OIS::KC_LEFT:
    case OIS::KC_A:
    if (idClient != "") {
        mDirection.x = -mMove/1.5; // on va a gauche
        mAnimationState->setEnabled(false);
        mAnimationState = mPlayer->getEntity()->getAnimationState("marcheGauche");
        mAnimationState->setLoop(true);
        mAnimationState->setEnabled(true);
        changement = true;
    }
    break;

    case OIS::KC_RIGHT:
    case OIS::KC_D:
    if (idClient != "") {
        mDirection.x = mMove/1.5; // on va a droite
        mAnimationState->setEnabled(false);
        mAnimationState = mPlayer->getEntity()->getAnimationState("marcheDroite");
        mAnimationState->setLoop(true);
        mAnimationState->setEnabled(true);
        changement = true;
    }
    break;

    case OIS::KC_PGUP:
    if (idClient != "") {
        mDirection.z = mMove/1.5; // on va en haut
        changement = true;
    }
    break;

    case OIS::KC_PGDOWN:
    if (idClient != "") {
        mDirection.z = -mMove/1.5; // on va en bas
        changement = true;
    }
    break;

    case OIS::KC_SPACE:
        if (launchIntShow)
        {
            pos=mPlayer->getSceneNode()->getPosition();
            posX = (float)pos.x;
            posY = (float)pos.y;
            posZ = (float)pos.z;
            if (distance(55.0,392.0,120.0,posX,posY,posZ,80.0,80.0,80.0) || distance(144.0,384.0,72.0,posX,posY,posZ,80.0,80.0,80.0))
                // television
            {
                showWindow(2,true);
            }

            else if (distance(-627.0,560.0,168.0,posX,posY,posZ,80.0,80.0,80.0) || distance(-752.0,472.0,168.0,posX,posY,posZ,80.0,80.0,80.0) ||
                     distance(736.0,-240.0,96.0,posX,posY,posZ,80.0,80.0,80.0) || distance(816.0,-160.0,96.0,posX,posY,posZ,80.0,80.0,80.0))
                //bibliotheque
            {
                showWindow(3,true);
            }

            else if (distance(224.0,-232.0,104.0,posX,posY,posZ,80.0,80.0,80.0))
                //lecteur audio
            {
                showWindow(4,true);
            }
        }
    break;

    default:
        break;
    }

    //return mContinue;
    return true;
}


bool VortexFrameListener::keyReleased(const OIS::KeyEvent &e)
{
    mGUI->injectKeyRelease(e);

    if (idClient != "") {
    switch (e.key) // ici on arrete le mouvement lorsque la touche est relachee
    {
    case OIS::KC_UP:
    case OIS::KC_W:
        mDirection.y = 0;
        if (mDirection.x==0)
        {
            mAnimationState->setEnabled(false);
        }
    break;

    case OIS::KC_DOWN:
    case OIS::KC_S:
        mDirection.y = 0;
        if (mDirection.x==0)
        {
            mAnimationState->setEnabled(false);
        }
    break;

    case OIS::KC_LEFT:
    case OIS::KC_A:
        mDirection.x = 0;
        mAngle=0;
        mAnimationState->setEnabled(false);//on désactive l'anim en cours (dans tous les cas)
        if (mDirection.y>0)
        {
            mAnimationState = mPlayer->getEntity()->getAnimationState("marcheAvant");
            mAnimationState->setLoop(true);
            mAnimationState->setEnabled(true);//on réactive l'anim dont on a besoin
        }
        else if (mDirection.y<0)
        {
            mAnimationState = mPlayer->getEntity()->getAnimationState("marcheArriere");
            mAnimationState->setLoop(true);
            mAnimationState->setEnabled(true);
        }
    break;

    case OIS::KC_RIGHT:
    case OIS::KC_D:
        mDirection.x = 0;
        mAngle=0;
        mAnimationState->setEnabled(false);
        if (mDirection.y>0)
        {
            mAnimationState = mPlayer->getEntity()->getAnimationState("marcheAvant");
            mAnimationState->setLoop(true);
            mAnimationState->setEnabled(true);//on réactive l'anim si besoin
        }
        else if (mDirection.y<0)
        {
            mAnimationState = mPlayer->getEntity()->getAnimationState("marcheArriere");
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

    bitBuffer buff;
    Vector3 pos;
    Quaternion dir;
    float posX;
    float posY;
    float posZ;
    float dirX = 0.0;
    float dirY = 0.0;
    int anim;
    pos=mPlayer->getSceneNode()->getPosition();
    dir = mPlayer->getSceneNode()->getOrientation();
    posX = (float)pos.x;
    posY = (float)pos.y;
    posZ = (float)pos.z;

    buff.writeFloat(posX); buff.writeFloat(posY); buff.writeFloat(posZ);
    buff.writeFloat(dirX); buff.writeFloat(dirY);
    buff.writeString(idClient);

    if (mAnimationState->getAnimationName() == "MarcheAvant") anim = 1;
    else if (mAnimationState->getAnimationName() == "MarcheArriere") anim = 2;
    else if (mAnimationState->getAnimationName() == "MarcheGauche") anim = 3;
    else if (mAnimationState->getAnimationName() == "MarcheDroite") anim = 4;
    else anim = 0;

    buff.writeInt(anim);
    ogreManagerEvent.sendEvent(8,buff);
    }

    return true;
}
