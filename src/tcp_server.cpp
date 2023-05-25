#include <tcp_server.hpp>
#define EXCEPTIONS 1
Asio_TCP_Server::Asio_TCP_Server(int PortNum):
    portNum(PortNum)
{
        isServerConnected= false;
        isThereAnyNewData= false;
        disconnectionTimeout = 100;
        // By default this is blocking
        isBlockingMode = true; 
}

Asio_TCP_Server::Asio_TCP_Server(int PortNumToBind,int timeOut,int backLogSize) :
    Asio_TCP_Server(PortNumToBind,backLogSize)
{
    disconnectionTimeout =timeOut;
    
}
Asio_TCP_Server::Asio_TCP_Server(int PortNumToBind, int BackLogsize): Asio_TCP_Server(PortNumToBind)
{
    backlogsize=BackLogsize;
}

void Asio_TCP_Server::WriteToClient(char * buffer, size_t sizeofBuffer) noexcept
{

    asio::error_code es;
    writeLock(_mtx);
    try
    {
        // listOfclients[0].non_blocking(isBlockingMode);
        if(listOfclients.size())
        {
            for(int i =0 ; i < listOfclients.size();i++)
            {
                listOfclients[i].write_some(asio::buffer(buffer, sizeofBuffer), es);
                if (es == asio::error::broken_pipe)
                {
                    DeallocateConnection(es);
                    throw std::runtime_error("TCP Server: Broken Pipe, Write failed, Deallocated Connection " + listOfclients[i].local_endpoint().address().to_string());
                }
                else
                {
                    // For completeness
                }
            }
        }
        
        
    }
    catch (const std::exception &e)
    {
        #if EXCEPTIONS ==1
        std::cerr << e.what() << '\n';
        #endif
    }
}


std::size_t  Asio_TCP_Server::ReadFromClient(char * buffer) noexcept
{
    receptionByteCount =-1;   
    asio::error_code es;
    readLock(_mtx);
    try
    {
        if(listOfclients.size())
        {
            for(int i= 0 ; i < listOfclients.size() ;i++)
            {
                if(listOfclients[i].available())
                {
                     std::cout<<"Reading bytes from  : "<<listOfclients[i].remote_endpoint().address().to_string()<<":"<<listOfclients[i].remote_endpoint().port()<<" bytes available :"<<listOfclients[i].available()<<std::endl;
                    receptionByteCount= listOfclients[i].read_some(asio::buffer(_receptionbuffer), es);
                    if (es && es == asio::error::eof)
                    {
                        isServerConnected =false;
                        listOfclients.pop_back();
                        throw std::runtime_error("TCP Server: Nothing to Read");
                    }		
                    buffer = _receptionbuffer.data();
                    last_message_read_time = std::chrono::system_clock::now();
                    //requires that client be read in order of added priority 
                    break;
                }
                else 
                {
                    no_message_read_time = std::chrono::system_clock::now();
                    size_t timediff = std::chrono::duration_cast<std::chrono::seconds>(no_message_read_time - last_message_read_time).count();
                    if(timediff > disconnectionTimeout)
                    {   
                        for(int i=0 ; i < listOfclients.size();i++)
                        {
                            receptionByteCount= listOfclients[i].read_some(asio::buffer(_receptionbuffer), es);
                            if(es == asio::error::eof)
                            {
                                throw std::runtime_error("TCP Server: Closing the socket" + listOfclients[i].local_endpoint().address().to_string());
                            }
                            listOfclients[i].close();
                            listOfclients.pop_back();
                        }
                    }
                }
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
    uniqueLock(_mtx);
    try
    {
        if((acceptor_.get()!=nullptr))
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
                    std::cout<<"Connected to : "<<socket_->remote_endpoint().address().to_string()
                        <<":"<<socket_->remote_endpoint().port()<<std::endl;
                    listOfclients.emplace_back(std::move(*socket_));
                    socket_.reset();
                }

                catch(const std::exception& e)
                {
                    socket_.reset();
                }
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
    socket_->close();
    socket_->release(es);
    acceptor_->close();
    isServerConnected = false;
    return;
}

void Asio_TCP_Server::ListenForConnections() 
{

    io_service = std::shared_ptr<asio::io_service>(new asio::io_service());
    acceptor_  = std::shared_ptr<asio::ip::tcp::acceptor>(new asio::ip::tcp::acceptor(*io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), portNum)));
    // Listen puts the accpetor into a state where it listens for multiple connections
    acceptor_->non_blocking(isBlockingMode);
    acceptor_->listen(backlogsize);

    return;
}

std::array<char , 1<<16> * Asio_TCP_Server::GetReadBufferPointer()
{
	return &(_receptionbuffer);
}

void Asio_TCP_Server::SetServerDisconnectionTimeout(int seconds)
{
    disconnectionTimeout = seconds;
}

bool Asio_TCP_Server::GetServerConnectionStatus()
{
    return isServerConnected;
}

Asio_TCP_Server::~Asio_TCP_Server()
{

}
