#include <tcp_server.hpp>

Asio_TCP_Server::Asio_TCP_Server(int PortNum):
    portNum(PortNum) ,  isServerConnected(false), isThereAnyNewData(false) 
{
       std::cout<<"------TCP Server intialising Port Number Assigned : " <<PortNum<<"------"<<std::endl;
}

void Asio_TCP_Server::start_service()
{
    std::cout<<"Listening & Replying mode "<<std::endl;
    std::cout<<"Connected to ----> Client ----> IP "<<socket_->remote_endpoint().address().to_string()<<std::endl;
    while(1)
    {
        asio::error_code es;
        asio::streambuf receive_buffer;

        //In all cases must recieve a byte atleast
        asio::read(*socket_,receive_buffer,asio::transfer_at_least(1),es);
        
        if(es== asio::error::eof )
        {
            std::cout<<"Connection closed, Client has disconnected  ... "<<std::endl;
            socket_->close();
            socket_->release(es);
            acceptor_->close();
            break;
        }
        else
        {
            const char *data = asio::buffer_cast<const char *>(receive_buffer.data());
            std::cout << data << std::endl;
            std::cout<<"Sending Acknowledgement" <<std::endl;
            const std::string msg = "ACK \n";
            //Assuming the socket is open please send an Acknowledgement for the message
            asio::write(*socket_,asio::buffer(msg),es);
            if(es== asio::error::broken_pipe )
            {
                std::cout<<"Connection closed, Client has disconnected  ... "<<std::endl;
                socket_->close();
                socket_->release(es);
                acceptor_->close();
                DeallocateConnection();
                break;
            }
        }
    }
    start_service();
}

void Asio_TCP_Server::WriteToClient(char * buffer, size_t sizeofBuffer)
{
    std::copy(buffer, buffer + sizeofBuffer , _broadcastbuffer.begin());
    std::cout<<_broadcastbuffer.data()<<std::endl;

    asio::error_code es;
    asio::write(*socket_,asio::buffer(_broadcastbuffer),es);
    if(es == asio::error::broken_pipe )
    {
        std::cout<<"Connection closed, Client has disconnected  ... "<<std::endl;
        socket_->close();
        socket_->release(es);
        acceptor_->close();
        DeallocateConnection();
    }
}




void Asio_TCP_Server::AcceptConnection()
{
    io_service =     new asio::io_service();
    acceptor_  =     new asio::ip::tcp::acceptor(*io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), portNum ));
    socket_    =     new asio::ip::tcp::socket(*io_service);
    acceptor_->accept(*socket_);
}

void Asio_TCP_Server::DeallocateConnection()
{
    delete [] acceptor_;
    delete [] io_service;
    delete [] socket_;
}

Asio_TCP_Server::~Asio_TCP_Server()
{

}

// Design goal of this TCP server 
// A typical asynchronous TCP server works according to the following algorithm:
// 1. This server intially connects to a single socket 
// 2. keeps connected with the client , incase the connection drops , remove the client from the list.
// 3. More to come
// 4. Even more to come
// 5. This has been tested on Xilinx SOMs aswell, and it works

//Improvements on 