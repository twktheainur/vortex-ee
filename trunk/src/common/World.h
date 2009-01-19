/* 
 * File:   world.h
 * Author: twk
 *
 * Created on January 11, 2009, 1:26 AM
 */

#ifndef _WORLD_H
#define	_WORLD_H
#include <vector>
#include "Object.h"
class World
{
public:
  World();
  World(const World& orig);
  virtual ~World();
private:
  vector<Object *> objects;
};

#endif	/* _WORLD_H */

