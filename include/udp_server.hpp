#ifndef _UDP_SERVER
#define _UDP_SERVER
#include "asio.hpp"
#include <memory>
#include <array>

class Asio_UDP_Server
{
    public:
        Asio_UDP_Server()=delete;
        Asio_UDP_Server(int portNumber, bool isBlockingMode=true);
        void WriteToClient(const char *buffer,size_t bufferSize,std::string && ipDestinationPort, int DestinationPort);
        int ReadFromClient();
        char * GetReadBufferPointer();

    private:

        bool isBlockingMode;

        std::array<char,1<<16> _receptionBuffer;

        asio::io_context _io_context;
        asio::ip::udp::socket _socket;

        std::string _destip;
        int _port;

};

#endif