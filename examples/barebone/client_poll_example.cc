#include <iostream>
#include "asio.hpp"

using namespace std;

int main()
{
    asio::io_service io_service;
    // socket creation
    asio::ip::tcp::socket socket(io_service);
    // connection
    socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 1234));
    // request/message from client
    const string msg = "Hello from Client!\n";
    asio::error_code error;
    while(1)
    {
        asio::write(socket, asio::buffer(msg), error);
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
            std::string_view message(foo.data(), x);
            std::cout << message << std::endl;
        }
    }
    return 0;
}