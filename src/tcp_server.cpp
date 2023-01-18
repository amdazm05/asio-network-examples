#include <tcp_server.hpp>
#define EXCEPTIONS 0
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
            // listOfclients[0].non_blocking(isBlockingMode);
            receptionByteCount= listOfclients[0].read_some(asio::buffer(_receptionbuffer), es);
            if (es && es != asio::error::eof)
            {
                if(listOfclients[0].is_open())
                    isServerConnected =false;
                listOfclients.pop_back();
                throw std::runtime_error("TCP Server: Nothing to Read");
            }		
            buffer = _receptionbuffer.data();
            if(receptionByteCount>0)
            {
            }
        }
        catch (const std::exception &e)
        {
            // #if EXCEPTIONS ==1
            std::cerr << e.what() << '\n';
            // #endif
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
                    std::cout<<"accepting again"<<std::endl;
                    asio::error_code es;
                     #if defined _WIN32 || defined WIN32 || defined OS_WIN64 || defined _WIN64 || defined WIN64 || defined WINNT
                    // use windows-specific time
                    int32_t timeout = 10000000;
                    setsockopt(socket_->native_handle(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
                    setsockopt(socket_->native_handle(), SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
                    #else
                    // assume everything else is posix
                    struct timeval tv;
                    tv.tv_sec  = timeout_milli / 1000;
                    tv.tv_usec = (timeout_milli % 1000) * 1000;
                    setsockopt(socket_->native_handle(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
                    setsockopt(socket_->native_handle(), SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
                    #endif

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
            // Do nothing pretty much
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
        // Listen puts the accpetor into a state where it listens for multiple connection
        // acceptor_->non_blocking(isBlockingMode);
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
