#include <tcp_server.hpp>

Asio_TCP_Server::Asio_TCP_Server(int PortNum):
    portNum(PortNum)
{
       std::cout<<"------TCP Server intialising Port Number Assigned : " <<PortNum<<"------"<<std::endl;
}

void Asio_TCP_Server::start_service()
{
    std::cout<<"Listening to ... "<<std::endl;
    asio::io_service io_service;
    asio::ip::tcp::socket socket(io_service);
    asio::ip::tcp::acceptor acceptor_(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), portNum ));
    acceptor_.accept(socket);
    std::cout<<"Connected to ----> Client ----> IP "<<socket.remote_endpoint().address().to_string()<<std::endl;
    while(1)
    {
        asio::error_code es;
        asio::streambuf receive_buffer;

        //In all cases must recieve a byte atleast
        asio::read(socket,receive_buffer,asio::transfer_at_least(1),es);
        
        if(es== asio::error::eof )
        {
            std::cout<<"Connection closed, Client has disconnected  ... "<<std::endl;
            socket.close();
            socket.release(es);
            acceptor_.close();
            break;
        }
        else
        {
            const char *data = asio::buffer_cast<const char *>(receive_buffer.data());
            std::cout << data << std::endl;
            std::cout<<"Sending Acknowledgement" <<std::endl;
            const std::string msg = "ACK \n";
            //Assuming the socket is open please send an Acknowledgement for the message
            asio::write(socket,asio::buffer(msg),es);
            if(es== asio::error::broken_pipe )
            {
                std::cout<<"Connection closed, Client has disconnected  ... "<<std::endl;
                socket.close();
                socket.release(es);
                acceptor_.close();
                break;
            }
        }
    }
    start_service();
}



// Design goal of this TCP server 
// A typical asynchronous TCP server works according to the following algorithm:
// 1. This server intially connects to a single socket 
// 2. keeps connected with the client , incase the connection drops , remove the client from the list.
// 3. More to come