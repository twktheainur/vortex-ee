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
    Vector3 anciennesCoord = mSceneNode->getPosition();
    mSceneNode->translate(direction * timeSinceLastFrame, Node::TS_LOCAL);//on "translate" le node du personnage
    
    if(_endroitInterdit(anciennesCoord, mSceneNode->getPosition()))//si le node se retrouve dans un endroit "interdit" (mur) à la suite de la translation,
      mSceneNode->translate(-(direction * timeSinceLastFrame), Node::TS_LOCAL); //on le retranslate, mais dans l'autre sens :p
      //ce double déplacement n'est pas visible, puisqu'il est fait entre 2 frames
  }
}

Real dist(Real x1, Real y1, Real x2, Real y2);//definie plus loin
//retourne la distance entre 2 points
//a utiliser modérément (contien un Sqrt (racine carrée : coute cher en CPU)

bool PersonnagePhysique::_endroitInterdit(Vector3 coordPrecedentes, Vector3 coord)
{
  //distance de 10 par rapport aux murs
  if(coord.x < -886 || coord.x > 802 || coord.y < -234 || coord.y > 554 //limites de la map (ordre g d b h , sur le plan vu de dessus dans gtkRadiant)
  || (coord.x < -738 && (coord.y < 82 || coord.y > 238 ) ) //a gauche et a droite de la porte du fond (ordre : haut, bas)
  || (coord.x > -370 && coord.x < -234 && coord.y > 70 && coord.y < 106 ) || (coord.x > -250 && coord.x < -214 && coord.y < 106 ) //cloison (a droite quand on arrive) (morceau horizontal _ , vertical | )
  || (coord.x > -138 && coord.x < -54 && coord.y > 214 && coord.y < 250 ) || (coord.x > -90 && coord.x < -54 && coord.y > 214 ) //cloison (a gauche quand on arrive) _ |
  || (coord.x < -718 && coord.y > 330 || coord.x < -522 && coord.y> 526 )// bibliotheque au dessus de l'entree (a gauche en arrivant) | _
  || (coord.x < -706 && coord.y < 2 || coord.x < -618 && coord.y < -194 )//meuble en dessous de l'entree | _
  || (dist(-380, 312, coord.x, coord.y) < 78)//table "octogonale" a gauche en rentrant
  || (coord.x > -466 && coord.x < -406 && coord.y > -194 && coord.y < -134)//petite table basse a droite en rentrant
  || (coord.z < 208 && coord.x > 542 && coord.x < 586 && coord.y > 222 && coord.y < 266 )//pilier vers cheminée
  || (coord.z < 208 && coord.x > 286 && coord.x < 330 && coord.y > -74 && coord.y < -30 )//pilier vers escalier
  || (coord.x > 586 && coord.y < -206 || coord.x > 782 && coord.y < -10 )//bibliotheque en bas a droite _ |
  || (coord.x > 682 && coord.y > 514 || coord.x > 770 && coord.y > 322 )//meuble en haut a droite (sur le plan vu de dessus toujours) _ |
   )
  {
    return true;
  }
  return false;// par défaut on retourne false ( = on ne se trouve pas dans un endroit interdit)
}

Real dist(Real x1, Real y1, Real x2, Real y2)
{
  Real x=x1-x2;
  Real y=y1-y2;
  return(Math::Sqrt(x*x+y*y)); 
}
