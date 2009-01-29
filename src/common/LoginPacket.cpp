#include "LoginPacket.h"
LoginPacket::LoginPacket(string login, string pass_hash)
{
  getData().writeString(login);
  getData().writeString(pass_hash);
  this->login=login;
  this->pass_hash=pass_hash;
  getHeader().writeChar(event_connect_login);
  getHeader().writeInt(getData().length());
}
LoginPacket::LoginPacket(char * buffer,size_t length)
{
  cout << "Ok now, i'm pissed off!"<<endl;
  bitBuffer tmp(buffer,length);
  cout << "I will have to kill somone for this!"<<endl;
  setHeader(tmp);
  cout << "I even might consider beheading!"<<endl;
  if(getHeader().readChar(true)!=event_connect_login)
    cout << "AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH!!!!!!!!!!!! HELP! INTRUDER ALERT!";
    //La ca par en couille il faut lancer une exception
    //C'est donc pas le bon paquet
    cout << "I can read no data!"<<endl;
    setDataSize(getHeader().readInt(true));
}
void LoginPacket::setData(char * buffer,size_t length)
{
  printf("setData start!\n");
  bitBuffer data(buffer,length);
  printf("bb generated\n");
  setDataBuffer(data);
  printf("data Buffer Set!\n");
  this->login = data.readString(true);
  this->pass_hash=data.readString(true);
  printf("setData end !\n");
}


