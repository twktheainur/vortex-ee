/*
 * BinBitSet.cpp
 *
 *  Created on: Dec 12, 2008
 *      Author: twk
 */

#include "BinBitSet.h"
#include <iostream>
BinBitSet::BinBitSet()
{
	byteOffset=1;
  vectorOffset=0;
  container.push_back(0);
}

BinBitSet::~BinBitSet()
{
	// TODO Auto-generated destructor stub
}

void BinBitSet::pushBit(bool bit)
{
	if(bit)
		container[vectorOffset]|=(1<<(byteOffset-1));//Setting the bit after the last set bit to 1
	else
		container[vectorOffset]&=~(1<<(byteOffset-1));//to 0
	byteOffset++;
	//If the current byte is full we allocate a new one
	//Resetting the byte Offset
	if(byteOffset>8)
	{
		std::cout<<"all";
		container.push_back(0);
		vectorOffset++;
		byteOffset=1;
	}
}

void BinBitSet::pushBits(std::string bits)
{
	//Anything other than 0 or 1 will set a bit to 1
  for(int i=0;i<bits.size();i++)
  {
  	printf("B:%d\n",bits[bits.size()-1-i]-'0');
    pushBit(bits.size()-1-i);
  }
}


bool BinBitSet::popBit()
{
	bool bit_val;
	//Getting the value of the last bit stored
	bit_val = (container[vectorOffset]&(1<<(byteOffset-1)))?1:0;
	byteOffset--;
	//If we reach the beginning of a byte,the said byte is empty and is therefore deallocated
	if(byteOffset==0)
	{
    vectorOffset--;
    byteOffset=8;
    container.pop_back();
	}
	return bit_val;
}

long BinBitSet::popBitsL(size_t n)
{
	long ret=0;
	if(n>sizeof(long)*8)
		return 0; //Overflow
  for(int i=0;i<n;i++)
  {
  	bool bit = popBit();
  	if(bit)
  		ret+=(1<<(i));
  }
  return ret;
}
std::string BinBitSet::popBitsS(size_t n)
{
  std::string ret;
  ret.resize(n);
  for(int i=0;i<n;i++)
  {
    ret[n-1-i]=popBit()+'0';
  }
  return ret;
}
