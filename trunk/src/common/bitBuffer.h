
#ifndef _BITBUFFER_H
#define _BITBUFFER_H

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

  bitBuffer()
  {
    bitIndex = 0;
  }

  explicit bitBuffer(char * pCharBuffer, long Length)
  {
    bitIndex = 0;
    //Load in the buffer to be used
    buffer.resize(Length);
    memcpy(&buffer[0], pCharBuffer, Length);
  }

  //WRITE FUNCTIONS

  void writeChar(char Value)
  {
    buffer.resize((bitIndex + (sizeof (char) *8) + 7) / 8);
    for (char copyBits = 0; (unsigned) copyBits < sizeof (char)*8; ++copyBits)
    {
      buffer[(bitIndex + copyBits) / 8] |= ((Value >> ((sizeof (char) *8 - 1) - copyBits)) & 0x1) << (7 - (bitIndex + copyBits) % 8);
    }
    bitIndex += sizeof (char) *8;
  }

  void writeShort(short Value)
  {
    buffer.resize((bitIndex + (sizeof (short) *8) + 7) / 8);
    for (char copyBits = 0; (unsigned) copyBits < sizeof (short) *8; ++copyBits)
    {
      buffer[(bitIndex + copyBits) / 8] |= ((Value >> ((sizeof (short) *8 - 1) - copyBits)) & 0x1) << (7 - (bitIndex + copyBits) % 8);
    }
    bitIndex += sizeof (short) *8;
  }

  void writeInt(int Value)
  {
    buffer.resize((bitIndex + (sizeof (int) *8) + 7) / 8);
    for (char copyBits = 0; (unsigned) copyBits < sizeof (int) *8; ++copyBits)
    {
      buffer[(bitIndex + copyBits) / 8] |= ((Value >> ((sizeof (int) *8 - 1) - copyBits)) & 0x1) << (7 - (bitIndex + copyBits) % 8);
    }
    bitIndex += sizeof (int) *8;
  }

  void writeBool(bool Value)
  {
    buffer.resize((bitIndex + 8) / 8);
    buffer[(bitIndex) / 8] |= (Value) << (7 - (bitIndex) % 8);
    ++bitIndex;
  }

  void writeFloat(float Value)
  {
    int Temp;
    memcpy(&Temp, &Value, sizeof (float));
    writeInt(Temp);
  }

 void  writeString(std::string Value)
  {
    writeChar(char(Value.size()));
    buffer.resize((bitIndex + ((Value.size())*8) + 7) / 8);
    for (std::string::iterator stringItr = Value.begin(); stringItr != Value.end(); ++stringItr)
    {
      for (char copyBits = 0; copyBits < 8; ++copyBits)
      {
        buffer[(bitIndex + copyBits) / 8] |= (((*stringItr) >> (7 - copyBits)) & 0x1) << (7 - (bitIndex + copyBits) % 8);
      }
      bitIndex += 8;
    }
  }


  //READ FUNCTIONS

  char readChar(bool Increment)
  {
    char Value = 0;
    for (char copyBits = 0; (unsigned) copyBits < sizeof (char) *8; ++copyBits)
    {
      Value <<= 1;
      Value |= (buffer[(bitIndex + copyBits) / 8] >> (7 - (bitIndex + copyBits) % 8)) & 0x1;
    }
    if (Increment) bitIndex += sizeof (char) *8;
    return Value;
  }

  int readInt(bool Increment)
  {
    int Value = 0;
    for (char copyBits = 0; (unsigned) copyBits < sizeof (int) *8; ++copyBits)
    {
      Value <<= 1;
      Value |= (buffer[(bitIndex + copyBits) / 8] >> (7 - (bitIndex + copyBits) % 8)) & 0x1;
    }
    if (Increment) bitIndex += sizeof (int) *8;
    return Value;
  }

  short readShort(bool Increment)
  {
    short Value = 0;
    for (char copyBits = 0; (unsigned) copyBits < sizeof (short) *8; ++copyBits)
    {
      Value <<= 1;
      Value |= (buffer[(bitIndex + copyBits) / 8] >> (7 - (bitIndex + copyBits) % 8)) & 0x1;
    }
    if (Increment) bitIndex += sizeof (short) *8;
    return Value;
  }

  bool readBool(bool Increment)
  {
    bool Value = (buffer[bitIndex / 8] >> (7 - bitIndex % 8)) & 0x1;
    if (Increment) ++bitIndex;
    return Value;
  }

  float readFloat(bool Increment)
  {
    int Temp = readInt(Increment);
    float Value;
    memcpy(&Value, &Temp, sizeof (float));
    return Value;
  }

  std::string readString(bool Increment)
  {
    std::string Value;
    long length = readChar(true);
    Value.resize(length);
    for (std::string::iterator stringItr = Value.begin(); stringItr != Value.end(); ++stringItr)
    {
      for (char copyBits = 0; copyBits < 8; ++copyBits)
      {
        (*stringItr) <<= 1;
        (*stringItr) |= (buffer[(bitIndex + copyBits) / 8] >> (7 - (bitIndex + copyBits) % 8)) & 0x1;
      }
      bitIndex += 8;
    }
    if (!Increment) bitIndex -= length * 8 + 8;
    return Value;
  }


  //RESET AND SET BITINDEX

  void resetBitIndex()
  {
    bitIndex = 0;
  }

  void setBitIndex(long Index)
  {
    bitIndex = Index;
  }

  long length()
  {
    return long(buffer.size());
  }

  char * retrieve()
  {
    return &buffer[0];
  }

  char * set()
  {
    return &buffer[0];
  }

  void resetBuffer()
  {
    for (std::vector<char>::iterator bufferItr = buffer.begin(); bufferItr != buffer.end(); ++bufferItr)
    {
      (*bufferItr) = 0;
    }
  }
};
//End class definition
#endif
