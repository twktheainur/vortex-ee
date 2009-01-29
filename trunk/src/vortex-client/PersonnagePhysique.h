#ifndef PERSONNAGE_PHYSIQUE_H
#define PERSONNAGE_PHYSIQUE_H

#include "Ogre.h"

using namespace Ogre;

//cette classe ressemble désormais plus a une structure qu'a une classe
//mais elle va nous permettre de ne pas changer toute l'implémentation de Application et VortexFrameListener

class PersonnagePhysique
{
public:
  PersonnagePhysique(SceneNode * mSceneNode, Entity * mEntity)
  {
    this->mSceneNode=mSceneNode;
    this->mEntity=mEntity;
    this->mSceneNode->attachObject(mEntity);
  }
  inline SceneNode * getSceneNode(){return mSceneNode;}
  inline Entity * getEntity(){return mEntity;}
private:
  SceneNode * mSceneNode;
  Entity * mEntity;
};


#endif
