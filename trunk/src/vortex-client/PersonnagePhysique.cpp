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
  if(coord.x < -880 || coord.x > 800 || coord.y < -230 || coord.y > 545 //limites de la map
  || (coord.x < -704 && (coord.y < 90 || coord.y > 224 ) ) //bibliotheques a droite et a gauche de la porte du fond
  || (coord.x > -370 && coord.x < -234 && coord.y > 70 && coord.y < 106 ) || (coord.x > -250 && coord.x < -214 && coord.y < 106 ) //cloison (a droite quand on arrive)
  || (coord.x > -138 && coord.x < -54 && coord.y > 214 && coord.y < 250 ) || (coord.x > -90 && coord.x < -54 && coord.y > 214 ) //cloison (a gauche quand on arrive)
   )
  {
    return true;
  }
  return false;// par défaut on retourne false ( = on ne se trouve pas dans un endroit interdit)
}
