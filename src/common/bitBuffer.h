/*
--BitBuffer--
Description - a buffer class for storing lists of data in the most efficient way.
--Credits--
-Created By Brandon Andrews
-Special Thanks to: Pfeilspitze and Mr_Awesome
*/

#ifndef _BITBUFFER_H
#define	_BITBUFFER_H
 
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
 
typedef unsigned char byte;
class bitBuffer
{
  private:
	std::vector<char> buffer;
	long bitIndex;
  public:
 
	union DoubleType
	{
		struct
		{
			int    first;
			int   second;
		};
		double Value;
	};
 
	union LongDoubleType
	{
		struct
		{
			int    first;
			int   second; 
			short	third;
		};
		long double Value;
	};
 
 bitBuffer(){
		bitIndex = 0;
	}
 
	bitBuffer(long packetSize){
		bitIndex = 0;
		buffer.resize(packetSize, 0);
	}
 
	explicit bitBuffer(char * pCharBuffer, long Length){
		bitIndex = 0;
		//Load in the buffer to be used
		buffer.resize(Length);
		memcpy(&buffer[0], pCharBuffer, Length);
	}
 
    //WRITE FUNCTIONS
	template <class T> void write(T Value, bool Resize){
		if(Resize) buffer.resize((bitIndex+(sizeof(T)*8)+7)/8);
		for(char copyBits = 0; copyBits < sizeof(T)*8; ++copyBits){
			buffer[(bitIndex+copyBits)/8] |= ((Value >> ((sizeof(T)*8-1)-copyBits)) & 0x1)<<(7-(bitIndex+copyBits)%8);
		}
		bitIndex += sizeof(T)*8;
	}
 
	template <class T> void writeArray(std::vector<T> & Value, bool Resize){
		char size = char(Value.size());
		write<char>(size,Resize);
		if(Resize) buffer.resize((bitIndex+(sizeof(T)*8*size)+7)/8);
		for(char cycleArray = 0; cycleArray < size; ++cycleArray){
			for(char copyBits = 0; copyBits < sizeof(T)*8; ++copyBits){
				buffer[(bitIndex+copyBits)/8] |= ((Value[cycleArray] >> ((sizeof(T)*8-1)-copyBits)) & 0x1)<<(7-(bitIndex+copyBits)%8);
			}
			bitIndex += sizeof(T)*8;
		}
	}
	//READ FUNCTIONS
	template <class T> T read(bool Increment){
		T Value = 0;
		for(char copyBits = 0; copyBits < sizeof(T)*8; ++copyBits){
			Value <<= 1;
			Value |= (buffer[(bitIndex+copyBits)/8]>>(7-(bitIndex+copyBits)%8)) & 0x1; 
		}
		if(Increment) bitIndex += sizeof(T)*8;
		return Value;
	}
 
	template <class T> std::vector<T> readArray(bool Increment){
		char size = read<char>(true);
		std::vector<T> Value;
		Value.resize(size);
		for(char cycleArray = 0; cycleArray < size; ++cycleArray){
			for(char copyBits = 0; copyBits < sizeof(T)*8; ++copyBits){
				Value[cycleArray] <<= 1;
				Value[cycleArray] |= (buffer[(bitIndex+copyBits)/8]>>(7-(bitIndex+copyBits)%8)) & 0x1; 
			}
			bitIndex += sizeof(T)*8;
		}
		if(!Increment) bitIndex -= sizeof(T)*8*size+8;
		return Value;
	}
 
	
 
	//RESET AND SET BITINDEX
	void resetBitIndex(){bitIndex = 0;}
	void setBitIndex(long Index){bitIndex = Index;}
 
	long length(){
		return long(buffer.size());
	}
 
	char * retrieve(){
		return &buffer[0];
	}
 
	char * set(){
		return &buffer[0];
	}
 
	void resetBuffer(){
		for(std::vector<char>::iterator bufferItr = buffer.begin(); bufferItr != buffer.end(); ++bufferItr){
			(*bufferItr) = 0;
		}
	}
};
//End class definition
  template <> void bitBuffer::write<bool>(bool Value, bool Resize) {
		if(Resize) buffer.resize((bitIndex+8)/8);
		buffer[(bitIndex)/8] |= (Value)<<(7-(bitIndex)%8);
		++bitIndex;
	}
  
  template <> void bitBuffer::write<float>(float Value, bool Resize) {
		int Temp;
		memcpy(&Temp,&Value,sizeof(float));
		write<int>(Temp, Resize);
	}
 
	template <> void bitBuffer::write<double>(double Value, bool Resize) {
    DoubleType ValueTemp = {{0}};
		ValueTemp.Value = Value;
		write<int>(ValueTemp.first, Resize);
		write<int>(ValueTemp.second, Resize);
	}
 
	template <> void bitBuffer::write<long double>(long double Value, bool Resize) {
    LongDoubleType ValueTemp ={{0}};
		ValueTemp.Value = Value;
		write<int>(ValueTemp.first, Resize);
		write<int>(ValueTemp.second, Resize);
		write<short>(ValueTemp.third, Resize);
	}
 
	template <> void bitBuffer::writeArray<float>(std::vector<float> & Value, bool Resize){
		int Temp;
		char size = char(Value.size());
		write<char>(size, Resize);
		if(Resize) buffer.resize((bitIndex+(sizeof(float)*8*size)+7)/8);
		for(char cycleArray = 0; cycleArray < size; ++cycleArray){
			memcpy(&Temp,&Value[cycleArray],sizeof(float));
			write<int>(Temp, Resize);
		}
	}
 
	template <> void bitBuffer::writeArray<double>(std::vector<double> & Value, bool Resize){
    DoubleType ValueTemp = {{0}};
		char size = char(Value.size());
		write<char>(size, Resize);
		if(Resize) buffer.resize((bitIndex+(sizeof(double)*8*size)+7)/8);
		for(char cycleArray = 0; cycleArray < size; ++cycleArray){
			ValueTemp.Value = Value[cycleArray];
			write<int>(ValueTemp.first, Resize);
			write<int>(ValueTemp.second, Resize);
		}
	}
 
	template <> void bitBuffer::writeArray<long double>(std::vector<long double> & Value, bool Resize){
    LongDoubleType ValueTemp = {{0}};
		char size = char(Value.size());
		write<char>(size, Resize);
		if(Resize) buffer.resize((bitIndex+(sizeof(long double)*8*size)+7)/8);
		for(char cycleArray = 0; cycleArray < size; ++cycleArray){
			ValueTemp.Value = Value[cycleArray];
			write<int>(ValueTemp.first, Resize);
			write<int>(ValueTemp.second, Resize);
			write<short>(ValueTemp.third, Resize);
		}
	}
 
	template <> void bitBuffer::write<std::string>(std::string Value, bool Resize) {
		write<char>(char(Value.size()), Resize);
		if(Resize) buffer.resize((bitIndex+((Value.size())*8)+7)/8);
		for(std::string::iterator stringItr = Value.begin(); stringItr != Value.end(); ++stringItr){
			for(char copyBits = 0; copyBits < 8; ++copyBits){
				buffer[(bitIndex+copyBits)/8] |= (((*stringItr) >> (7-copyBits)) & 0x1)<<(7-(bitIndex+copyBits)%8);
			}
			bitIndex += 8;
		}
	}

  
  template <> bool bitBuffer::read<bool>(bool Increment){
		bool Value = (buffer[bitIndex/8]>>(7-bitIndex%8)) & 0x1; 
		if(Increment) ++bitIndex;
		return Value;
	}
 
	template <> float bitBuffer::read<float>(bool Increment){ 
		int Temp = read<int>(Increment);
		float Value;
		memcpy(&Value, &Temp, sizeof(float));
		return Value;
	}
 
	template <> double bitBuffer::read<double>(bool Increment){ 
		DoubleType Value;
		Value.first = read<int>(true);
		Value.second = read<int>(true);
		if(!Increment) bitIndex -= 64;
		return Value.Value;
	}
 
	template <> long double bitBuffer::read<long double>(bool Increment){ 
		LongDoubleType Value;
		Value.first = read<int>(true);
		Value.second = read<int>(true);
		Value.third = read<short>(true);
		if(!Increment) bitIndex -= 80;
		return Value.Value;
	}
 
	template <> std::vector<float> bitBuffer::readArray<float>(bool Increment){
		int Temp;
		char size = read<char>(true);
		std::vector<float> Value;
		Value.resize(size);
		for(char cycleArray = 0; cycleArray < size; ++cycleArray){
			Temp = read<int>(true);
			memcpy(&Value[cycleArray], &Temp, sizeof(float));
		}
		if(!Increment) bitIndex -= sizeof(float)*8*size+8;
		return Value;
	}
 
	template <> std::vector<double> bitBuffer::readArray<double>(bool Increment){
		DoubleType Temp;
		char size = read<char>(true);
		std::vector<double> Value;
		Value.resize(size);
		for(char cycleArray = 0; cycleArray < size; ++cycleArray){
			Temp.first = read<int>(true);
			Temp.second = read<int>(true);
			Value[cycleArray] = Temp.Value;
		}
		if(!Increment) bitIndex -= sizeof(double)*8*size+8;
		return Value;
	}
 
	template <> std::vector<long double> bitBuffer::readArray<long double>(bool Increment){
		LongDoubleType Temp;
		char size = read<char>(true);
		std::vector<long double> Value;
		Value.resize(size);
		for(char cycleArray = 0; cycleArray < size; ++cycleArray){
			Temp.first = read<int>(true);
			Temp.second = read<int>(true);
			Temp.third = read<short>(true);
			Value[cycleArray] = Temp.Value;
		}
		if(!Increment) bitIndex -= sizeof(long double)*8*size+8;
		return Value;
	}
 
	template <> std::string bitBuffer::read<std::string>(bool Increment){ 
		std::string Value;
		long length = read<char>(true);
		Value.resize(length);
		for(std::string::iterator stringItr = Value.begin(); stringItr != Value.end(); ++stringItr){ 
			for(char copyBits = 0; copyBits < 8; ++copyBits){
				(*stringItr) <<= 1;
				(*stringItr) |= (buffer[(bitIndex+copyBits)/8]>>(7-(bitIndex+copyBits)%8)) & 0x1; 
			}
			bitIndex+=8;
		}
		if(!Increment) bitIndex -= length*8 + 8;
		return Value;
	}

#endif	/* _BITBUFFER_H */

