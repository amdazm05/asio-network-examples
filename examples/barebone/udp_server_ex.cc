#include <udp_server.hpp>
int main()
{
    std::string broadCastMessage = "DEADBEEFBEEFFEED";
    const char* _bufferPtr = broadCastMessage.c_str();
    Asio_UDP_Server Server(9100);
    while(1)
    {
        Server.WriteToClient(_bufferPtr,broadCastMessage.length(),"127.0.0.1",9002);
    }

    return {};
}