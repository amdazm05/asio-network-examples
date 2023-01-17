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
    // request/message from client
    std::fstream file;
    std::array<char,1<<16> msg= {};
    file.open("PacketJson.bin",std::ios::binary| std::ios::in | std::ios::out);
    asio::error_code error;
    file.read(msg.begin(),1855);

    asio::write(socket, asio::buffer(msg,1855), error);
    if (!error)
    {
        cout << "Client sent hello message!" << endl;
    }
    else
    {
        cout << "send failed: " << error.message() << endl;
    }
    char buff[]= "Hello";
    while(1)
    {
        asio::write(socket, asio::buffer(buff,4), error);
        if (!error)
        {
            cout << "Client sent hello message!" << endl;
        }
        else
        {
            cout << "send failed: " << error.message() << endl;
        }
        // getting response from server
        asio::streambuf receive_buffer;
        std::array<char, 1<<16> foo;
        std::size_t x = socket.read_some(asio::buffer(foo), error);

        if (error && error != asio::error::eof)
        {
            cout << "receive failed: " << error.message() << endl;
        }
        else
        {
            // std::string_view message(foo.data(), x);
            // std::cout << message << std::endl;
            std::cout<<"Length of bytes recieved "<<x<<std::endl;
            for(int i=0; i<x;i++)
                printf("%c",*(foo.data()+i));
            std::cout<<std::endl;

        }
    }
    return 0;
}