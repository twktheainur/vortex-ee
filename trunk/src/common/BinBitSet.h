/*
 * BinBitSet.h
 *
 *  Created on: Dec 12, 2008
 *      Author: twk
 */

#ifndef BINBITSET_H_
#define BINBITSET_H_
#include <bitset>
#include <vector>
#include <string>
using namespace std;
class BinBitSet
{
private:
	int byteOffset;
	long vectorOffset;
	std::vector<unsigned char> container;
public:
	void pushBit(bool bit);
	bool popBit();
	string popBitsS(size_t n);
	long popBitsL(size_t n);
	void pushBits(std::string bits);
	void pushBits(long val,size_t n);
	BinBitSet();
	virtual ~BinBitSet();
	inline const unsigned char * getContainerPtr(){return (const unsigned char *)&container;}
	inline vector<unsigned char> & getContainer(){return container;}
	inline size_t  getBitCount(){return (8*(vectorOffset)+(size_t)byteOffset-1);}
	inline int getByteOffset(){return byteOffset;}
	inline long getByteCount(){return (vectorOffset);}
};

#endif /* BINBITSET_H_ */
