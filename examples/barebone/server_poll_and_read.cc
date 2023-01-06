#include <iostream>
#include "asio.hpp"
#include <string.h>
#include <tcp_server.hpp>

using namespace std;
using namespace asio;

int main()
{

    Asio_TCP_Server Server(1234);
    char *buffer = "1321312";

    while (1)
    {
        Server.ListenForConnections();
        Server.AcceptConnection();
        Server.listen_and_reply_once(buffer, 4);
    }
    //       Server.start_service();

    return 0;
}