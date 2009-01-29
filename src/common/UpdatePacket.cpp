#include "UpdatePacket.h"
#include "bitBuffer.h"
UpdatePacket::UpdatePacket(float px,float py,float pz, float dx, float dy,string id)
{
  getData().writeFloat(px);
  getData().writeFloat(py);
  getData().writeFloat(pz);
  getData().writeFloat(dx);
  getData().writeFloat(dy);
  getData().writeString(id);
  setDataSize(getData().length());
  this->id=id;
  getHeader().writeChar(event_world_update);
  getHeader().writeInt(getData().length());
}
UpdatePacket::UpdatePacket(char * buffer,size_t length)
{
  bitBuffer tmp =bitBuffer(buffer,length);
  setHeader(tmp);
  if(getHeader().readChar(false)!=event_world_update)
    cout;
  setType(getHeader().readChar(true));
  setDataSize(getHeader().readInt(true));
    //La ca part en couille il faut lancer une exception
    //C'est donc pas le bon paquet
}
void UpdatePacket::setData(char * buffer,size_t length)
{
  bitBuffer data(buffer,length);
  setDataBuffer(data);
  this->id = data.readString(true);
}
