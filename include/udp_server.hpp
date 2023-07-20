#ifndef _UDP_SERVER
#define _UDP_SERVER
#include "asio.hpp"
#include <memory>
#include <array>

class Asio_UDP_Server
{
    public:
        Asio_UDP_Server()=delete;
        Asio_UDP_Server(int portNumber, std::string && ip);
        void WriteToClient(std::shared_ptr<char> buffer,size_t bufferSize);
        int ReadFromClient();
        char * GetReadBufferPointer();

    private:

        bool isBlockingMode;

        std::array<char,1<<16> _receptionBuffer;

        asio::io_context _io_context;
        asio::ip::udp::socket _socket;

};

#endif