#ifndef _TJSON
#define _TJSON
#include <iostream>
#include <array>

struct Threats
{
  
    uint64_t currentSystemIteration;
    uint64_t TOAFirst;
    uint64_t TOALast;
    char * Band;
    uint64_t AOA;
    uint64_t PW;
    uint64_t PRI;
    uint64_t RF;
};

class NetworkPacketLogger
{
    public:
        NetworkPacketLogger();
        void parseThreat
        (
            uint64_t currentSystemIteration,
            uint64_t TOAFirst,
            uint64_t TOALast,
            char * Band,
            uint64_t AOA,
            uint64_t PW,
            uint64_t PRI,
            uint64_t RF
        );

        void tracksConversionToJSON();
        void clearBuffer();
        const char * getBuffer() const;
        const size_t getBufferSize();
    private:
        int lengthofString=0;
        std::array<Threats,100>  listOfreportedThreats;
        int threatCountinList=0;
        //1MB in JSON packet
        std::array<char, 1<<20 > buffer;
};

#endif //_TJSON