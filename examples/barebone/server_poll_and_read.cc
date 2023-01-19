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
    char  readbuffer[100] = {};
    Server.ListenForConnections();
    while (1)
    {
        Server.AcceptConnection();
        // Server.listen_and_reply_once(buffer, 4);
        Server.ReadFromClient(readbuffer);
        std::cout<<readbuffer<<std::endl;
        Server.WriteToClient(buffer,4);

    }
    //       Server.start_service();

    return 0;
}