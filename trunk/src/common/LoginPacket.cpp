#include "LoginPacket.h"
LoginPacket::LoginPacket(string login, string pass_hash)
{
  getData().writeString(login);
  getData().writeString(pass_hash);
  this->login=login;
  this->pass_hash=pass_hash;
  getHeader().writeChar(Event::event.connect.login);
  getHeader().writeInt(getData().length());
}
LoginPacket::LoginPacket(char * buffer,size_t length)
{
  bitBuffer tmp =bitBuffer(buffer,length);
  setHeader(tmp);
  if(getData().readInt(true)!=Event::event.connect.login)
    cout;
    //La ca par en couille il faut lancer une exception
    //C'est donc pas le bon paquet
}
void LoginPacket::setData(char * buffer,size_t length)
{
  bitBuffer data = bitBuffer(buffer,length);
  setDataBuffer(data);
  this->login = data.readString(true);
  this->pass_hash=data.readString(true);
}


