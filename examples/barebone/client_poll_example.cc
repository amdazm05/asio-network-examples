#include <iostream>
#include <fstream>
#include "asio.hpp"

using namespace std;

int main()
{
    asio::io_service io_service;
    // socket creation
    asio::ip::tcp::socket socket(io_service);
    // connection
    socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 9000));
    socket.non_blocking(true);
    asio::error_code es;
    asio::socket_base::keep_alive option(true);
    socket.set_option(option,es);

    asio::ip::tcp::socket::linger option_linger;
    option_linger.enabled(true);
    option_linger.timeout(1000);
    socket.set_option(option_linger);
    // request/message from client
    std::fstream file;
    char msg[100]= "HELLO SERVER";
    asio::error_code error;



    while(1)
    {
        asio::write(socket, asio::buffer(msg,sizeof(msg)), error);
        if (!error)
        {

        }
        else
        {

        }

        asio::streambuf receive_buffer;
        std::array<char, 1<<16> foo;

        std::size_t x = socket.read_some(asio::buffer(foo), error);
        if (error && error != asio::error::eof)
        {

        }
        else
        {
            std::cout<<"Length of bytes recieved "<<x<<std::endl;
            for(int i=0; i<x;i++)
                printf("%c",*(foo.data()+i));
            std::cout<<std::endl;

        }
    }
    return 0;
}