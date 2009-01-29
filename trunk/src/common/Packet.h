/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#ifndef PACKET_H_
#define PACKET_H_
//#include <bitset>
#include <cstdlib>
#include <string>
#include <vector>
#include "bitBuffer.h"
#include "Event.h"
//#include "BinBitSet.h"
using namespace std;
class Packet
{
public:
 //Dans le premier constructeur de chaque paquet on verifiera bien que c'est le bon paquet!
 //Voir setData() pour mettre le reste
 //Le deuxieme constructeur prendra les champs du paquet en paramentre et construira un
 //paquet entier
  virtual ~Packet(){}
  virtual void setData(char * buffer,size_t length)=0;//Ajoute les donnees au paquet
  inline bitBuffer & getData(){return data;}
  inline bitBuffer & getHeader(){return header;}
  inline void setHeader(bitBuffer &head){header=head;}
  inline void setDataBuffer(bitBuffer &data){this->data=data;}
  inline size_t getPacketSize()
  {
  	return getHeaderSize() + getDataSize();
  }
  inline size_t getHeaderSize(){return header.length();}
  inline size_t getDataSize(){return dataSize;}
  inline void setType(int t){type=t;}
  inline void setDataSize(size_t s){dataSize=s;}


private:
  bitBuffer data;
  bitBuffer header;
  size_t dataSize;
  int type;
};

#endif /* PACKET_H_ */
