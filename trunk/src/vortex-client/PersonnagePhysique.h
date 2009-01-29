#ifndef PERSONNAGE_PHYSIQUE_H
#define PERSONNAGE_PHYSIQUE_H

#include "Ogre.h"

using namespace Ogre;

class PersonnagePhysique
{
public:
  PersonnagePhysique(SceneNode * mSceneNode, Entity * mEntity);//constructeur
  inline SceneNode * getSceneNode() const {return mSceneNode;}//getter
  inline Entity * getEntity() const {return mEntity;}//getter
  void deplacement(Vector3 direction, Real timeSinceLastFrame);//déplace le personnage dans la direction indiquée, si c'est possible
private:
  SceneNode * mSceneNode;
  Entity * mEntity;
  bool _endroitInterdit(Vector3 coord);
};


#endif
