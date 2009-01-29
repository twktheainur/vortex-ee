/*
 * LoginPacket.h
 *
 *  Created on: Dec 17, 2008
 *      Author: twk
 */

#ifndef LOGINPACKET_H_
#define LOGINPACKET_H_
#include "Packet.h"

class LoginPacket: public Packet
{
public:
      ~LoginPacket(){}
	  LoginPacket(string login, string pass_hash);
	  LoginPacket(char * buffer,size_t length);
	  void setData(char * buffer,size_t length);
	  inline string getLogin(){return login;}
	  inline string getPassHash(){return pass_hash;}
private:
  string login;
  string pass_hash;
};

#endif /* LOGINPACKET_H_ */
