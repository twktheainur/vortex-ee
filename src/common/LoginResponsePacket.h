/*
 * LoginPacket.h
 *
 *  Created on: Dec 17, 2008
 *      Author: twk
 */

#ifndef LOGINRESPONSEPACKET_H_
#define LOGINRESPONSEPACKET_H_
#include "Packet.h"

class LoginResponsePacket: public Packet
{
public:
	  LoginResponsePacket(string sessid);
	  LoginResponsePacket(char * buffer,size_t length);
	  void setData(char * buffer,size_t length);
private:
  string sessid;
};

#endif /* LOGINPACKET_H_ */
