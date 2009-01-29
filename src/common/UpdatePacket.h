#ifndef UPDATEPACKET_H_INCLUDED
#define UPDATEPACKET_H_INCLUDED
#include "Packet.h"
class UpdatePacket : public Packet
{
    public:
      ~UpdatePacket(){}
	  UpdatePacket(float px,float py,float pz, float dx, float dy,string id);
	  UpdatePacket(char * buffer,size_t length);
	  void setData(char * buffer,size_t length);
private:
  string id;
  float px;
  float py;
  float pz;
  float dx;
  float dy;
};
#endif // UPDATEPACKET_H_INCLUDED
