#include "PersonnagePhysique.h"

PersonnagePhysique::PersonnagePhysique(SceneNode * mSceneNode, Entity * mEntity)
{
    this->mSceneNode=mSceneNode;
    this->mEntity=mEntity;
    this->mSceneNode->attachObject(mEntity);
}

void PersonnagePhysique::deplacement(Vector3 direction, Real timeSinceLastFrame)
{
  if(direction!=Vector3::ZERO)//c'est a dire, "si on veut se deplacer" (sinon on ne fait rien)
  {
    Vector3 anciennesCoord = mSceneNode->getPosition();
    mSceneNode->translate(direction * timeSinceLastFrame, Node::TS_LOCAL);//on "translate" le node du personnage
    
    if(endroitInterdit(anciennesCoord, mSceneNode->getPosition()))//si le node se retrouve dans un endroit "interdit" (mur) à la suite de la translation,
      mSceneNode->translate(-(direction * timeSinceLastFrame), Node::TS_LOCAL); //on le retranslate, mais dans l'autre sens :p
      //ce double déplacement n'est pas visible, puisqu'il est fait entre 2 frames
  }
  Vector3 coord = mSceneNode->getPosition();
  ajusterAltitude(coord, timeSinceLastFrame);//on ajuste l'altitude même s'il n'y a pas eu de mouvement volontaire : il s'agit d'une "chute"
  mSceneNode->setPosition(coord);
}

Real dist(Real x1, Real y1, Real x2, Real y2);//definie plus loin
//retourne la distance entre 2 points
//a utiliser modérément (contien un Sqrt (racine carrée : coute cher en CPU)

bool PersonnagePhysique::endroitInterdit(Vector3 coordPrecedentes, Vector3 coord)
//retourne vrai si et seulement si le personnage se trouve dans un endroite ou il n'a pas le droit d'être
//on utilise les coordonnées précédentes dans le cas ou il y a un décalage de niveau du sol :
//par exemple l'endroit ou se trouve la TV est un endroit interdit si on arrive par en bas
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
  || (coord.x > 726 && coord.y > 42 && coord.y < 270 )//cheminee
  || (coord.x > 182 && coord.x < 266 && coord.y < -206)//chaine hi-fi
  || (coord.x < 138 && coordPrecedentes.x >= 138 && ( coordPrecedentes.y < 126 || coordPrecedentes.y > 194))//"niveau superieur" (on ne peut pas remonter par là, il faut prendre l'escalier ... && (min escalier || max esc)
  || (coord.x < 138 && coord.x > 64 && ( coord.y < 126 && coordPrecedentes.y >=126 || coord.y > 194 && coordPrecedentes.y <= 194) )//on est dans l'escalier et on essaye de remonter sur les bords (pas bien)
  || (coord.x > 118 && coord.x < 166 && coord.y > 342 && coord.y < 434)//"magnetoscope" (enfin, le truc en bois devant la télé)
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

void PersonnagePhysique::ajusterAltitude(Vector3 & coord, Real timeSinceLastFrame)
{
  if(coord.x < 26 || coord.x < 138 && (coord.y < 116 || coord.y > 204 ))//partie de gauche (entrée), || cotés de l'escalier
  {
    coord.z = 127;
  }
  else if(coord.y > 124 && coord.y < 194 && coord.x > 26 && coord.x < 138)//petit escalier
  {
    if(coord.x < 50)//marche la plus haute
    {
      if(coord.z < 114.9 )//on est trop bas
        coord.z = 115;
      else if(coord.z > 115.1)// on est trop haut
        coord.z -= timeSinceLastFrame * 200;
    }
    else if(coord.x < 74)//2° marche
    {
      if(coord.z < 102.9 )//on est trop bas
        coord.z = 113;
      else if(coord.z > 113.1)// on est trop haut
        coord.z -= timeSinceLastFrame * 200;
    }
    else if(coord.x < 98 )//3° marche
    {
      if(coord.z < 90.9 )//on est trop bas
        coord.z = 91;
      else if(coord.z > 91.1)// on est trop haut
        coord.z -= timeSinceLastFrame * 200;
    }
    else if (coord.x < 122 )//4°marche
    {
      if(coord.z < 78.9 )//on est trop bas
        coord.z = 79;
      else if(coord.z > 79.1)// on est trop haut
        coord.z -= timeSinceLastFrame * 200;
    }
    else//niveau du bas
    {
      if(coord.z<70.9)
      coord.z=71;
    else if (coord.z > 71.1)
      coord.z -= timeSinceLastFrame*200;
    }
    
  }
  else if(coord.x >= 138)//partie "basse" (vers la cheminée)
  {
    //remarque dans les conditions suivantes j'ai mis 70.9 et 71.1 car avec 71 on observait un leger "tremblement"
    //a mon avis la hauteur (z) était très légèrement mofidifiée lors de déplacements "horizontaux" a chaque frame, donc elle n'était plus égale à 71, et ca réajustait
    if(coord.z<70.9)//ca ne devrait pas arriver : ca voudrait dire qu'on est sous le sol
    //mais ca peut arriver si la chute est pas assez précise et qu'on traverse le sol l'instant d'une frame
      coord.z=71;
    else if (coord.z > 71.1)
      coord.z -= timeSinceLastFrame*200;//ce déplacement est linéaire : admettons que nous sommes dans un univers parallèle où la gravité se comporte différemment :p
    //si il est déja à 71 on fait rien
  }
}
