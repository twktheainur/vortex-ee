/*
 * File:   world.h
 * Author: twk
 *
 * Created on January 11, 2009, 1:26 AM
 */

#ifndef _WORLD_H
#define	_WORLD_H
#include <vector>
#include <iostream>
using namespace std;


typedef struct world_user
{
  string id;
  float px;
  float py;
  float pz;
  float dx;
  float dy;
}world_user_t;

class World
{
public:
  World();
  World(const World& orig);
  virtual ~World();
  inline void add_user(string id)
  {
    world_user_t user={id,-680,160,127,1,0};
    users.push_back(user);
  }
  inline void delete_user(string id)
  {
    unsigned int i=0;
    while (i < users.size() && users[i].id != id)
    {
      i++;
    }
    if(i<=users.size())
      users.erase(users.begin()+i);
  }
  inline void update_user(world_user_t user,string id)
  {
    unsigned int i=0;
    while (i < users.size() && users[i].id != id)
    {
      i++;
    }
    if(i<=users.size())
      users[i]=user;
    else
     cout;
     //TODO: exception...
  }
private:
  vector<world_user_t> users;
};

#endif	/* _WORLD_H */

