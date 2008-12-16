#ifndef BINBITSET_H_
#define BINBITSET_H_
#include <bitset>
#include <vector>
#include <string>
#include <iostream>
using namespace std;
class BinBitSet
{
private:
	int byteOffset;
	long vectorOffset;
	std::vector<unsigned char> container;
public:
	void pushBit(bool bit);
	//void pushBitsS(std::string bits);
	//void pushBits(long val,size_t n);
	template <class T>
	void pushBits(T var)
	{
		//Adding each bit of var to the set
		bool bval;
		size_t n = sizeof(T)*8;
    for(size_t i=0;i<n;i++)
    {
    	std::cout<< i<<endl;
    	bval=(var&(1<<i))?1:0;
      pushBit(bval);
    }
	}
	template <class T>
	T popBits()
	{
		T ret=0;
		size_t n = sizeof(T)*8;
	  for(int i=0;i<n;i++)
	  	if(popBit())
	  		ret+=(1<<(n-i));
	  return ret;
	}

	bool popBit();
	//string popBitsS(size_t n);
	long popBitsL(size_t n);

	BinBitSet(vector<unsigned char> vect);
	BinBitSet();
	virtual ~BinBitSet();

	inline const unsigned char * getDataPtr(){return (const unsigned char *)&container;}
	inline vector<unsigned char> & getContainer(){return container;}
	vector<unsigned char> getSubBinBitSet(size_t start, size_t length);

	inline size_t  getBitCount(){return (8*(vectorOffset)+(size_t)byteOffset-1);}
	inline int getByteOffset(){return byteOffset;}
	inline long getByteCount(){return (vectorOffset);}
};

#endif /* BINBITSET_H_ */
