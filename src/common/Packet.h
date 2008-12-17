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

  inline size_t getPacketSize()
  {
  	return getHeaderSize() + getDataSize();
  }
  virtual size_t getHeaderSize()=0;
  virtual size_t getDataSize()=0;

private:
  BinBitSet container;
};

#endif /* PACKET_H_ */
