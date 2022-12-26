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
    private:
        int portNum;
        asio::streambuf _receptionbuffer;
        asio::streambuf _broadcastbuffer;
};

#endif

// References here
//http://think-async.com/Asio/Download.html