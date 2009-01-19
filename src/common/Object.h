/* 
 * File:   object.h
 * Author: twk
 *
 * Created on January 11, 2009, 1:27 AM
 */

#ifndef __OBJECT_H_
#define	__OBJECT_H_
#include <string>
class Object
{
public:
  Object();
  Object(const Object& orig);
  virtual std::string getId()=0;
  inline void setPos(int x,int y,int z)
  {
    posX=x;posY=y;posZ=z;
  }
  inline void setDir(int x,int y, int z)
  {
    dirX=x;dirY=y;dirZ=z;
  }
  virtual ~Object();
protected:
  inline int getInstances(){return __instances;}
private:
  static int __instances;
  int posX;
  int posY;
  int posZ;
  int dirX;
  int dirY;
  int dirZ;
};
int Object::__instances=0;

#endif	/* _OBJECT_H */

