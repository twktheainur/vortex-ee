#include "LoginResponsePacket.h"
LoginResponsePacket::LoginResponsePacket(string sessid)
{
  getData().writeString(sessid);
  this->sessid=sessid;
  getHeader().writeChar(event_connect_login);
  getHeader().writeInt(getData().length());
}
LoginResponsePacket::LoginResponsePacket(char * buffer,size_t length)
{
  bitBuffer tmp =bitBuffer(buffer,length);
  setHeader(tmp);
  if(getData().readChar(false)!=event_connect_login)
    cout;
  setType(getData().readChar(true));
  setDataSize(getData().readInt(true));
    //La ca par en couille il faut lancer une exception
    //C'est donc pas le bon paquet
}
void LoginResponsePacket::setData(char * buffer,size_t length)
{
  bitBuffer data = bitBuffer(buffer,length);
  setDataBuffer(data);
  this->sessid = data.readString(true);
}

