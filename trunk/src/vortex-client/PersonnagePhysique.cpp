#include "PersonnagePhysique.h"

PersonnagePhysique::PersonnagePhysique(SceneNode * mSceneNode, Entity * mEntity)
{
    this->mSceneNode=mSceneNode;
    this->mEntity=mEntity;
    this->mSceneNode->attachObject(mEntity);
}

void PersonnagePhysique::deplacement(Vector3 direction, Real timeSinceLastFrame)
{
  if(direction!=Vector3::ZERO)//c'est a dire, "si on veut se deplacer" (sinon on fait rien)
  {
    mSceneNode->translate(direction * timeSinceLastFrame, Node::TS_LOCAL);//on "translate" le node du personnage
    
    if(_endroitInterdit(mSceneNode->getPosition()))//si le node se retrouve dans un endroit "interdit" (mur) à la suite de la translation,
      mSceneNode->translate(-(direction * timeSinceLastFrame), Node::TS_LOCAL); //on le retranslate, mais dans l'autre sens :p
      //ce double déplacement n'est pas visible, puisqu'il est fait entre 2 frames
  }
}

bool PersonnagePhysique::_endroitInterdit(Vector3 coord)
{
  if(coord.x < -880 || coord.x > 800 || coord.y < -230 || coord.y > 545 )
  {
    return true;
  }
  return false;// par défaut on retourne false ( = on ne se trouve pas dans un endroit interdit)
}
