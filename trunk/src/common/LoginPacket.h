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
	  LoginPacket(string login, string pass_hash);
	  LoginPacket(BinBitSet set);

	  inline size_t getHeaderSize(){return 3 + 8 + 8;}
	  size_t getDataSize(){return login.size()+pass_hash.size();}


	  inline bool isLoginPacket()
	  {

	  }
private:
  int type; // For isLoginPacket
  string login;
  string pass_hash;
};

#endif /* LOGINPACKET_H_ */
