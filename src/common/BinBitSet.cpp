/*
 * BinBitSet.cpp
 *
 *  Created on: Dec 12, 2008
 *      Author: twk
 */

#include "BinBitSet.h"
#include <iostream>
BinBitSet::BinBitSet(vector<unsigned char> vect)
{
	byteOffset=1;
  vectorOffset=0;
  container=vect;
  container.push_back(0);
}
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
  for(int i=0;i<=n;i++)
  {
  	bool bit = popBit();
  	if(bit)
  		ret+=(1<<(n-i));
  }
  return ret;
}
//Get a subset of the container vector
vector<unsigned char> BinBitSet::getSubBinBitSet(size_t start,size_t length)
{
	vector <unsigned char> new_cont;
	std::vector<unsigned char>::iterator it;
	if(start<=container.size())
	{
    for(it = container.begin();
        *it!=container[start] &&    it!=container.end() &&    *it!=container[length]; it++);

	  if(it!=container.end())
      new_cont.assign(it,(container.size()>=length)?container.end():container.begin()+start+length);
	}
	return new_cont;
}
