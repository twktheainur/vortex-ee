/* 
 * File:   avatar.cpp
 * Author: twk
 * 
 * Created on January 11, 2009, 1:37 AM
 */

#include "Avatar.h"
#include "Object.h"
#include <iostream>

Avatar::Avatar()
{
  std::stringstream str(Avatar::objectId);
  str << getInstances();
  instanceId=str.str();
}

Avatar::~Avatar()
{
}

