#include "PersonnagePhysique.h"

PersonnagePhysique::PersonnagePhysique(const String& name, Vector3 dimensions) : ApplicationObject(name)
{
  mDimensions=dimensions;//vous inquietez pas il y un constructeur de copie par defaut dans la classe Ogre::Vector3
  //cet a dire que l'operateur "=" copie vraiment les valeurs de l'objet, pas juste un pointeur
  setUp(name);
}

PersonnagePhysique::~PersonnagePhysique(){}

void PersonnagePhysique::setUp(const String& name)
{
  // Create visual presence
  SceneManager* sm = World::getSingleton().getSceneManager();
  mEntity = sm->createEntity(name, "man.mesh");
  mSceneNode = sm->getRootSceneNode()->createChildSceneNode(name,Vector3(-680,160,127));
  mSceneNode->attachObject(mEntity);
  //mSceneNode->pitch(Degree(90));//a cause des axes de Ogre vs Quake 3
  //mSceneNode->yaw(Degree(90));
  //mSceneNode->scale(Vector3(2,2,2));
  //mSceneNode->setFixedYawAxis(true, Vector3::UNIT_Z);// on redresse l'axe de la node egalement
  // Add reverse reference
  mEntity->setUserObject(this);

  // Create mass body
  mOdeBody = new dBody(World::getSingleton().getOdeWorld()->id());
  // Set reverse reference
  mOdeBody->setData(this);
  // Set mass 
  setMassBox(10000, mDimensions);

  this->setBounceParameters(0.0, 0.0);//on veut pas qu'il rebondisse
  this->setSoftness(0.0f);
  this->setFriction(Math::POS_INFINITY);

  // Create collision proxy
  dBox* odeBox = new dBox(0, mDimensions.x, mDimensions.y, mDimensions.z);
  mCollisionProxies.push_back(odeBox);
  updateCollisionProxies();
}
