#include "PersonnagePhysique.h"

PersonnagePhysique::PersonnagePhysique(SceneNode * mSceneNode, Entity * mEntity)
{
    this->mSceneNode=mSceneNode;
    this->mEntity=mEntity;
    this->mSceneNode->attachObject(mEntity);
}

void PersonnagePhysique::deplacement(Vector3 direction)
{
  mSceneNode->translate(direction, Node::TS_LOCAL); 
}
