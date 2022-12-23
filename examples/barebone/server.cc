#include <iostream>
#include "asio.hpp"
#include <string.h>
#include <tcp_server.hpp>

using namespace std;
using namespace asio;

string read_(ip::tcp::socket & socket) {
       asio::streambuf buf;
       asio::read_until( socket, buf, "\n" );
       string data = asio::buffer_cast<const char*>(buf.data());
       return data;
}

void send_(ip::tcp::socket & socket, const string& message) {
       const string msg = message + "\n";
       asio::write( socket, asio::buffer(message) );
}

int main() {

      Asio_TCP_Server Server(1234);
      Server.start_service();

   return 0;
}