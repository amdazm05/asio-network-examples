#include <tcp_server.hpp>
#define EXCEPTIONS 1
Asio_TCP_Server::Asio_TCP_Server(int PortNum):
    portNum(PortNum)
{
        isServerConnected= false;
        isThereAnyNewData= false;
        // By default this is blocking
        isBlockingMode = true; 
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
            // listOfclients[0].non_blocking(isBlockingMode);
            listOfclients[0].write_some(asio::buffer(buffer, sizeofBuffer), es);
            if (es == asio::error::broken_pipe)
            {
                DeallocateConnection(es);
                throw std::runtime_error("TCP Server: Broken Pipe, Write failed, Deallocated Connection");
            }
        }
        catch (const std::exception &e)
        {
            #if EXCEPTIONS ==1
            std::cerr << e.what() << '\n';
            #endif
        }
    }
}


std::size_t  Asio_TCP_Server::ReadFromClient(char * buffer) noexcept
{
    receptionByteCount =-1;   
    if(isServerConnected)
    {
        asio::error_code es;
        try
        {
            if(listOfclients[0].available())
            {
                receptionByteCount= listOfclients[0].read_some(asio::buffer(_receptionbuffer), es);
                if (es && es == asio::error::eof)
                {
                    isServerConnected =false;
                    listOfclients.pop_back();
                    throw std::runtime_error("TCP Server: Nothing to Read");
                }		
                buffer = _receptionbuffer.data();
                last_message_read_time = std::chrono::system_clock::now();
            }

            else 
            {
                no_message_read_time = std::chrono::system_clock::now();
                size_t timediff = std::chrono::duration_cast<std::chrono::seconds>(no_message_read_time - last_message_read_time).count();
                std::cout<<timediff<<std::endl;
                if(timediff > 10)
                {   
                    isServerConnected = false;
                    listOfclients[0].close();
                    listOfclients.pop_back();
                }

            }
            
        }
        catch (const std::exception &e)
        {
            #if EXCEPTIONS ==1
            std::cerr << e.what() << '\n';
            #endif
            receptionByteCount = -1;
        }
    }
   
    return receptionByteCount;
}

char* Asio_TCP_Server::listen_and_reply_once(char * bufferToWrite, size_t sizeofBufferToWrite) noexcept
{
    
    asio::error_code es;
    try
    {
        if(isServerConnected)
        {
            receptionByteCount = listOfclients[0].read_some(asio::buffer(_receptionbuffer), es);
            if (es && es != asio::error::eof)
            {
                isServerConnected =false;
                listOfclients.pop_back();
                throw std::runtime_error("TCP Server: Socket Closed");
            }
        }
            
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        isServerConnected = false;
        return nullptr;
    }
    
    try
    {
        if(isServerConnected)
        {   
            asio::write(listOfclients[0],asio::buffer(bufferToWrite,sizeofBufferToWrite),es);
            if(es == asio::error::broken_pipe )
            {
                throw std::runtime_error("TCP Server: Broken Pipe, Write failed, Deallocated Connection");
            }
        }            
    }
    catch(const std::exception& e)
    {
        #if EXCEPTIONS ==1
        std::cerr << e.what() << '\n';
        #endif
        isServerConnected = false;
        return nullptr;
    }

    return _receptionbuffer.data();
}


void Asio_TCP_Server::AcceptConnection() noexcept
{
    try
    {
        if((acceptor_.get()!=nullptr) && !isServerConnected)
        {   
                socket_ = std::shared_ptr<asio::ip::tcp::tcp::socket>(new asio::ip::tcp::tcp::socket(*io_service));
                try
                {
                    asio::error_code accept_error;
                    // ::system::error_code ec;
                    acceptor_->accept(*socket_);
                    asio::error_code es;
                    asio::socket_base::keep_alive option(true);
                    socket_->set_option(option,es);

                    asio::ip::tcp::socket::linger option_linger;
                    option_linger.enabled(true);
                    option_linger.timeout(1000);
                    socket_->set_option(option_linger);

                    socket_->non_blocking(true);
                    last_message_read_time = std::chrono::system_clock::now();

                    isServerConnected = true;
                    listOfclients.emplace_back(std::move(*socket_));
                    socket_.reset();
                }

                catch(const std::exception& e)
                {
                    socket_.reset();
                }
        }
        else if(isServerConnected)
        {

        }
        else
        {
            throw std::runtime_error("TCP Server: Incorrect usage of Accept Connection - Please listen before accepting \n");
        }
    }
    catch(const std::exception& e)
    {
        #if EXCEPTIONS ==1
        std::cerr << e.what() << '\n';
        #endif
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
        io_service = std::shared_ptr<asio::io_service>(new asio::io_service());
        acceptor_  = std::shared_ptr<asio::ip::tcp::acceptor>(new asio::ip::tcp::acceptor(*io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), portNum)));
        // Listen puts the accpetor into a state where it listens for multiple connections
        acceptor_->non_blocking(isBlockingMode);
        acceptor_->listen(backlogsize);
        return;
    }

    else
    {
        return;
    }
}

std::array<char , 1<<16> * Asio_TCP_Server::GetReadBufferPointer()
{
	return &(_receptionbuffer);
}

bool Asio_TCP_Server::GetServerConnectionStatus()
{
    return isServerConnected;
}

Asio_TCP_Server::~Asio_TCP_Server()
{

}
