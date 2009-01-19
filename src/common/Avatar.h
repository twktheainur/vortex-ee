/* 
 * File:   avatar.h
 * Author: twk
 *
 * Created on January 11, 2009, 1:37 AM
 */

#ifndef _AVATAR_H
#define	_AVATAR_H

class Avatar:public Object
{
public:
  Avatar();
  ~Avatar(){}
private:
  static const string objectId;
  string instanceId;
};
Avatar::objectId="__avatar";

#endif	/* _AVATAR_H */

