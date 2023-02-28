#include <iostream>
#include <thread>
#include "asio.hpp"
#include <string.h>
#include <chrono>
#include <tcp_server.hpp>

using namespace std;
using namespace asio;
Asio_TCP_Server Server(9000);
char *buffer_write = "1321312";
char  *readbuffer;

void WriteToClient()
{
    while(1)
    {
        Server.WriteToClient(buffer_write,4);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


void ReadFromClient()
{
    while(1)
    {
        Server.AcceptConnection();
        int bytesreturned= Server.ReadFromClient(readbuffer);
        if(bytesreturned>1)
        {
            for(int i=0 ; i< bytesreturned;i++)
                printf("%c" , (char * )Server.GetReadBufferPointer()->data()[i]);
            std::cout<<"\n";
        }
    }
        
}

int main()
{
    Server.ListenForConnections();
    std::thread t1(&ReadFromClient);
    std::thread t2(&WriteToClient);

    t1.join();
    t2.join();
    return 0;
}