#include "udp_server.hpp"

Asio_UDP_Server::Asio_UDP_Server(int portNumber, std::string && ip) :
    _socket(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(),portNumber))
{

}

void Asio_UDP_Server::WriteToClient(std::shared_ptr<char> buffer,size_t bufferSize)
{
    
}
int Asio_UDP_Server::ReadFromClient()
{

}
char * Asio_UDP_Server::GetReadBufferPointer()
{

}