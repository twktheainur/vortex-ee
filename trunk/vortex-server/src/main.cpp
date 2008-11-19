/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#include <cstdlib>
#define _REENTRANT
//#include "TCPServer.h"
#include "TCPClient.h"
#include <time.h>
#include <string.h>
#include <pthread.h>

void * client_thread(void * socket)
{
  TCPSocket local_socket = *((TCPSocket*)socket);
  string buffer;
  buffer.resize(150);
  buffer[149]='\0';
  printf("%s:%s Connected.",local_socket.getHost().data(),local_socket.getService().data());
  string html="<h1 style=\"color:red;\">Welcome on my TCPServer!</h1><p>Google is your friend!</p><a href='http://google.com'>Google!</a>";
  try
  {
    html.resize(200);
    local_socket >> buffer;
    printf("|Recieved Data: %s|\n",buffer.data());
    if(time(NULL)%2==0)
    {
      local_socket << "AC";
      local_socket << html;
    }
    else
      local_socket << "RJ";



  }
  catch(exception * e)
  {
    printf("Client Thread(recv):%s\n",e->what());
    delete e;
  }
  return (void *)NULL;
}

void client_handler(TCPSocket socket)
{
  void * tmp_ptr=(void *)&socket;
pthread_t * thread;
pthread_create(thread,NULL,client_thread,tmp_ptr);
   //client_thread(tmp_ptr);
}

int main(int argc, char **argv)
{
  string smsg="HELO";
  size_t sz = smsg.size();
  string rmsg; rmsg.resize(3);
  rmsg[2]='\0';
  try
  {
    //TCPServer my_serv(5,"8080",client_handler);
    //my_serv.startListener();
    TCPClient my_cli("localhost","8080");
    my_cli.send(smsg.data(),&sz,0);
    my_cli.recv((void *)rmsg.data(),rmsg.size()-1,0);
    cout << rmsg << "|"<< endl;
    if(strncmp(rmsg.data(),"AC",2)==0)
    {
      rmsg.resize(151);
      rmsg[150]='\0';
      my_cli.recv((void *)rmsg.data(),rmsg.size()-1,0);
      cout << "Getting data:|" << rmsg << "|" << endl;
    }
    else
      cout << "Rejected by server" << endl;
  }
  catch(exception * e)
  {
    printf("whatm:%s\n",e->what());
    delete e;
  }

  return EXIT_SUCCESS;
}
