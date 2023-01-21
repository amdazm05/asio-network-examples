#ifndef __TCP_SERVER
#define __TCP_SERVER
#include <iostream>
#include <vector>
#include <array>

// Disable one of these depending on what compiler you're working with
#define ASIO_STANDALONE 
#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_TYPE_TRAITS
//including asio header only implementation
#include "asio.hpp"


class Asio_TCP_Server
{
    public:
        Asio_TCP_Server() = delete;
        // Alsos delegating constructor
        Asio_TCP_Server(int PortNumToBind);
        Asio_TCP_Server(int PortNumToBind, int BackLogsize);
        char * listen_and_reply_once(char * bufferToWrite, size_t sizeofBufferToWrite) noexcept;
        void AcceptConnection() noexcept;
        void SetBackLogSize();
        void ListenForConnections();
        void DeallocateConnection(asio::error_code &es);
        void WriteToClient(char * buffer, size_t sizeofBuffer) noexcept;
        std::size_t  ReadFromClient(char * buffer) noexcept;
		std::array<char , 1<<16> * GetReadBufferPointer();
        void SetBlockingCall(bool condition);
        bool GetServerConnectionStatus();
        ~Asio_TCP_Server();
    private:
        int portNum;
        bool isServerConnected;
        bool isThereAnyNewData;    
        bool isBlockingMode;
        int  backlogsize;

        // Connection poll timer 
        std::chrono::time_point<std::chrono::system_clock> no_message_read_time;
        std::chrono::time_point<std::chrono::system_clock> last_message_read_time;

        std::array<char , 1<<16> _broadcastbuffer;
        std::array<char , 1<<16> _receptionbuffer;
        std::size_t receptionByteCount;
        
        std::shared_ptr<asio::io_service> io_service;
        std::shared_ptr<asio::ip::tcp::socket> socket_;
        std::shared_ptr<asio::ip::tcp::acceptor> acceptor_;
        std::vector<asio::ip::tcp::socket> listOfclients;
};

#endif

// References here
// http://think-async.com/Asio/Download.html
// https://subscription.packtpub.com/book/application-development/9781783986545/1/ch01lvl1sec16/accepting-connections
// https://stackoverflow.com/questions/63917790/c-boost-asio-how-to-detect-disconnection-and-make-a-list-of-all-active-conne