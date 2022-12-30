#ifndef _TJSON
#define _TJSON
#include <iostream>
#include <vector>
#include <array>

struct PulsedThreats
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

struct CWThreats
{
  
    uint64_t currentSystemIteration;
    char * Band;
    uint64_t Angle;
    uint64_t Quad;
    uint64_t heading;
    uint64_t tA;
    uint64_t tb;
    uint64_t tc;
    uint64_t tD;
    uint64_t cwGainStage;
    uint64_t RF;
};

// This can be set as 
template<class DataAnalysisClass, class TrackDefaultClass>
class JSONLoggerDelegate
{
    public:
        JSONLoggerDelegate();
        
        void PulsedTrackConversion
        (
            DataAnalysisClass &analysisObj ,  uint64_t currentIteration
        );

        void CWTrackConversion
        (
            std::vector<TrackDefaultClass> &tracks ,  uint64_t currentIteration
        );

        void tracksConversionToJSON();
        void clearBuffer();
         char * getBuffer() ;
         size_t getBufferSize();
    private:
        int lengthofString=0;
        std::array<PulsedThreats,50>  listOfreportedThreats;
        std::array<CWThreats,50>  listOfreportedCWThreats;
        int threatCountinList=0;
        int CWthreatCountinList=0;
        //1MB in JSON packet
        std::array<char, 1<<20 > buffer;
};

#endif //_TJSON