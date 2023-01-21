#include <iostream>
#include "asio.hpp"
#include <string.h>
#include <tcp_server.hpp>

using namespace std;
using namespace asio;

int main()
{

    Asio_TCP_Server Server(9000);
    char *buffer = "1321312";
    char  *readbuffer;
    Server.ListenForConnections();
    readbuffer = (char * )Server.GetReadBufferPointer();

    while (1)
    {
        Server.AcceptConnection();
        // Server.listen_and_reply_once(buffer, 4);
        int bytesreturned= Server.ReadFromClient(readbuffer);
        Server.WriteToClient(buffer,4);
        if(bytesreturned>1)
        {

            std::cout<<bytesreturned<<std::endl;
            std::cout<<std::endl;
        }
       
    }
    //       Server.start_service();

    return 0;
}