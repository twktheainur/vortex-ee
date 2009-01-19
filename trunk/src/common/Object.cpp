/* 
 * File:   object.cpp
 * Author: twk
 * 
 * Created on January 11, 2009, 1:27 AM
 */

#include "Object.h"

Object::Object()
{
   Object::__instances++;
}
Object::~Object()
{
  Object::__instances--;
}

