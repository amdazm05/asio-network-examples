#ifndef __TCP_SERVER
#define __TCP_SERVER
#include <iostream>
#include <vector>
#include <array>

#define ASIO_STANDALONE 
#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_TYPE_TRAITS
//including asio header only implementation
#include "asio.hpp"


class Asio_TCP_Server
{
    public:
        Asio_TCP_Server() = delete;
        Asio_TCP_Server(int PortNumToBind );
        void start_service();
        void AcceptConnection();
        void DeallocateConnection();
        void WriteToClient(char * buffer, size_t sizeofBuffer);
        ~Asio_TCP_Server();
    private:
        int portNum;
        bool isServerConnected;
        bool isThereAnyNewData;        
        asio::streambuf _receptionbuffer;
        std::array<char , 1<<16> _broadcastbuffer;

        asio::io_service *io_service;
        asio::ip::tcp::socket *socket_;
        asio::ip::tcp::acceptor *acceptor_;
};

#endif

// References here
//http://think-async.com/Asio/Download.html