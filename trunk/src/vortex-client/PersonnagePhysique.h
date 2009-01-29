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
  bool _endroitInterdit(Vector3 coordPrecedentes, Vector3 coord);//retourne vrai si on n'a pas le droit de se trouver à cet endroit
  //on utilise les "coordonnées précedentes", car pour certains endroits, on a pas le droit de s'y trouver si on vient d'un endroit précis
  //par ex : si on est en bas, devant la TV, on ne peut pas se retrouver au niveau de la TV (plus haut) comme par magie (il y un truc qui s'appelle la gravité qui empêche ca :o) )
};


#endif
