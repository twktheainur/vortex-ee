#ifndef __PERSONNAGE_PHYSIQUE_H__
#define __PERSONNAGE_PHYSIQUE_H__

#include "OgreRefAppPrerequisites.h"
#include "OgreRefAppApplicationObject.h"

namespace OgreRefApp
{

  class _OgreRefAppExport PersonnagePhysique : public ApplicationObject
  {
  public:
    PersonnagePhysique(const String& name, Vector3 dimensions=Vector3(100,200,600));//valeur par defaut a changer pour correspondre a la taille du perso
    ~PersonnagePhysique();
  private:
    Vector3 mDimensions;//sert à définir une "bounding box" qui servira pour les collisions
    // ca ne sera pas très réaliste, mais je ne sais pas comment définir des formes plus complexes,
    // et de toutes facons on a pas le temps pour des formes plus complexes
    void setUp(const String& name);
  };

}
#endif
