#ifndef __TCP_SERVER
#define __TCP_SERVER
#include <iostream>
#include <vector>
#include <array>

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