#include "ThreatJSONschema.hpp"

void NetworkPacketLogger::parseThreat
(
            uint64_t currentSystemIteration,
            uint64_t TOAFirst,
            uint64_t TOALast,
            char * Band,
            uint64_t AOA,
            uint64_t PW,
            uint64_t PRI,
            uint64_t RF

)

{

    //pushes the track into the vector
    if(threatCountinList<=listOfreportedThreats.size())
    {
        listOfreportedThreats[threatCountinList].currentSystemIteration = currentSystemIteration;
        listOfreportedThreats[threatCountinList].TOAFirst= TOAFirst;
        listOfreportedThreats[threatCountinList].TOALast=TOALast;
        listOfreportedThreats[threatCountinList].Band = Band;
        listOfreportedThreats[threatCountinList].AOA =AOA;
        listOfreportedThreats[threatCountinList].PW = PW;
        listOfreportedThreats[threatCountinList].PRI =PRI;
        listOfreportedThreats[threatCountinList].RF = RF;
        threatCountinList++;
    }
    //To keep track of how many tracks we got
     
}



void NetworkPacketLogger::tracksConversionToJSON()
{
    lengthofString = 0;
    lengthofString+=sprintf(buffer.begin(),"{[");
    for(int i=0; i<threatCountinList;i++)
    {
        if(i!=(threatCountinList-1))
            lengthofString += sprintf(buffer.begin() + lengthofString, "{\"TOAFirst\":%lld,\"TOALast\":%lld \"Band\":\"%s\",\"AOA\":%lld, \"PW\":%lld,\"PRI\":%lld, \"RF\":%lld},",
                                      listOfreportedThreats[threatCountinList].currentSystemIteration,
                                      listOfreportedThreats[threatCountinList].TOAFirst,
                                      listOfreportedThreats[threatCountinList].TOALast,
                                      listOfreportedThreats[threatCountinList].Band,
                                      listOfreportedThreats[threatCountinList].AOA,
                                      listOfreportedThreats[threatCountinList].PW,
                                      listOfreportedThreats[threatCountinList].PRI,
                                      listOfreportedThreats[threatCountinList].RF

            );
        else
            lengthofString += sprintf(buffer.begin() + lengthofString, "{\"TOAFirst\":%lld,\"TOALast\":%lld \"Band\":\"%s\",\"AOA\":%lld, \"PW\":%lld,\"PRI\":%lld, \"RF\":%lld}]}",
                                      listOfreportedThreats[threatCountinList].currentSystemIteration,
                                      listOfreportedThreats[threatCountinList].TOAFirst,
                                      listOfreportedThreats[threatCountinList].TOALast,
                                      listOfreportedThreats[threatCountinList].Band,
                                      listOfreportedThreats[threatCountinList].AOA,
                                      listOfreportedThreats[threatCountinList].PW,
                                      listOfreportedThreats[threatCountinList].PRI,
                                      listOfreportedThreats[threatCountinList].RF);
    }

}


void NetworkPacketLogger::clearBuffer()
{
    listOfreportedThreats = {};
    lengthofString  = 0;
}

inline const char * NetworkPacketLogger::getBuffer() const
{
    return buffer.data();
}

inline const size_t NetworkPacketLogger::getBufferSize()
{
    return lengthofString;
}