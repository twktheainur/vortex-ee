/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/

#include "ClientManager.h"

ClientManager::ClientManager()
{
  // TODO Auto-generated constructor stub

}

ClientManager::~ClientManager()
{
  // TODO Auto-generated destructor stub
}

friend void client_handler(TCPSocket socket)
{
  void * tmp_ptr=(void *)&socket;
  pthread_create(clientThread,NULL,client_thread,tmp_ptr);
}

friend void * client_thread_handler(void * arg)
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
