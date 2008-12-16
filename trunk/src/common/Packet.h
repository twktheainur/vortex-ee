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
#include "BinBitSet.h"
using namespace std;
class Packet
{
public:
  Packet();
  virtual ~Packet();


  virtual size_t getPacketSize()=0;
  virtual size_t getHeaderSize()=0;
  virtual size_t getTailSize()=0;
  virtual size_t getDataSize()=0;

  virtual void make_header()=0;
  inline vector<unsigned char>
  getHeader()
  {
  	return container.getSubBinBitSet(0,getHeaderSize());
  }

  void pack();
  inline vector<unsigned char>
  unpack()
  {
  	return container.getSubBinBitSet(getHeaderSize(),getDataSize());
  }

  virtual void make_tail()=0;
  inline vector<unsigned char>
  getTail()
  {
  	return container.getSubBinBitSet(getHeaderSize()+getDataSize(),getTailSize());
  }

private:
  BinBitSet container;
};

#endif /* PACKET_H_ */
