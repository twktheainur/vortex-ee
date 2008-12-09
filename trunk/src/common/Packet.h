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
#include <bitset>
using namespace std;
class Packet
{
public:
  Packet();
  void pack();
  void unpack();
  virtual void make_header()=0;
  virtual void make_tail()=0;
  size_t getPacketSize();
  size_t getHeaderSize();
  size_t getTailSize();
//  bitset & getHeader();
//  bitset & getData();
//  bitset & getTail();
  virtual ~Packet();
private:
//  bitset header;
  string data;
//  bitset tail;
};

#endif /* PACKET_H_ */
