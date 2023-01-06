#include <tcp_server.hpp>

Asio_TCP_Server::Asio_TCP_Server(int PortNum):
    portNum(PortNum)
{
        isServerConnected= false;
        isThereAnyNewData= false; 
}

Asio_TCP_Server::Asio_TCP_Server(int PortNumToBind, int BackLogsize): Asio_TCP_Server(PortNumToBind)
{
    backlogsize=BackLogsize;
}

void Asio_TCP_Server::WriteToClient(char * buffer, size_t sizeofBuffer) noexcept
{
    if(isServerConnected)
    {
        asio::error_code es;
        try
        {
            asio::write(*socket_, asio::buffer(buffer, sizeofBuffer), es);
            if (es == asio::error::broken_pipe)
            {
                DeallocateConnection(es);
                throw std::runtime_error("TCP Server: Broken Pipe, Write failed, Deallocated Connection");
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

char* Asio_TCP_Server::listen_and_reply_once(char * bufferToWrite, size_t sizeofBufferToWrite) noexcept
{
    asio::error_code es;

    try
    {
        receptionByteCount = socket_->read_some(asio::buffer(_receptionbuffer), es);
        if (es && es != asio::error::eof)
        {
            DeallocateConnection(es);
            throw std::runtime_error("TCP Server: Socket Closed");
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return nullptr;
    }
    
    try
    {
        asio::write(*socket_,asio::buffer(bufferToWrite,sizeofBufferToWrite),es);
        if(es == asio::error::broken_pipe )
        {
            DeallocateConnection(es);
            throw std::runtime_error("TCP Server: Broken Pipe, Write failed, Deallocated Connection");
        }

        return _receptionbuffer.data();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return nullptr;
    }

}




void Asio_TCP_Server::AcceptConnection() noexcept
{
    try
    {
        if(acceptor_!=nullptr)
        {
            if(!isServerConnected)
            {
                acceptor_->accept(*socket_);
                isServerConnected = true;
            }
        }
        else
        {
            throw std::runtime_error("TCP Server: Incorrect usage of Accept Connection - Please listen before accepting \n");
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return;
}

void Asio_TCP_Server::DeallocateConnection(asio::error_code &es)
{
    if(isServerConnected)
    {
        socket_->close();
        socket_->release(es);
        acceptor_->close();
        isServerConnected = false;
        return;
    }
    else
    {
        return;
    }
    
}

void Asio_TCP_Server::ListenForConnections() 
{
    if(!isServerConnected)
    {
        io_service =     new asio::io_service();
        acceptor_  =     new asio::ip::tcp::acceptor(*io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), portNum ));
        acceptor_->listen(backlogsize);
        return;
    }

    else
    {
        return;
    }
}

Asio_TCP_Server::~Asio_TCP_Server()
{

}
