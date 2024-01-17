#include "udp_server.hpp"

Asio_UDP_Server::Asio_UDP_Server(int portNumber, bool isBlockingMode):
    _socket(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(),portNumber)),_port(portNumber)
{
    _socket.non_blocking(isBlockingMode);
}

void Asio_UDP_Server::WriteToClient(const char * buffer,size_t bufferSize, std::string && ipDestinationPort, int destinationPort)
{
    asio::ip::udp::endpoint endpoint(asio::ip::address::from_string(ipDestinationPort), destinationPort);
    _socket.send_to(asio::buffer(buffer,bufferSize),endpoint);
}
int Asio_UDP_Server::ReadFromClient()
{   
    return {};
}
char * Asio_UDP_Server::GetReadBufferPointer()
{
    return nullptr;
}