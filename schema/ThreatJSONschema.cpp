#include "ThreatJSONschema.hpp"

template<class DataAnalysisClass>
void NetworkPacketLogger<DataAnalysisClass>::PulsedTrackConversion
(
            DataAnalysisClass &analysisObj ,  uint64_t currentIteration
)

{
    uint16_t i=0;
    for(; i<analysisObj.bandEJ.tracks.size();i++)
    {
        listOfreportedThreats[i].currentSystemIteration =   currentIteration;
        listOfreportedThreats[i].TOAFirst=                  analysisObj.bandEJ.tracks[i].toaFirst;
        listOfreportedThreats[i].TOALast=                   analysisObj.bandEJ.tracks[i].toaLast;
        listOfreportedThreats[i].Band =                     "EJ";
        listOfreportedThreats[i].AOA =                      analysisObj.bandEJ.tracks[i].angle.bearingLast;
        listOfreportedThreats[i].PW =                       analysisObj.bandEJ.tracks[i].pwAvg;
        listOfreportedThreats[i].PRI =                      analysisObj.bandEJ.tracks[i].priMean;
        listOfreportedThreats[i].RF =                       analysisObj.bandEJ.tracks[i].rfAvg;
    }

    for(uint16_t j =0; j<analysisObj.bandCD.tracks.size();j++)
    {

        listOfreportedThreats[i].currentSystemIteration =   currentIteration;
        listOfreportedThreats[i].TOAFirst=                  analysisObj.bandCD.tracks[j].toaFirst;
        listOfreportedThreats[i].TOALast=                   analysisObj.bandCD.tracks[j].toaLast;
        listOfreportedThreats[i].Band =                     "CD";
        listOfreportedThreats[i].AOA =                      analysisObj.bandCD.tracks[j].angle.bearingLast;
        listOfreportedThreats[i].PW =                       analysisObj.bandCD.tracks[j].pwAvg;
        listOfreportedThreats[i].PRI =                      analysisObj.bandCD.tracks[j].priMean;
        listOfreportedThreats[i].RF =                       analysisObj.bandCD.tracks[j].rfAvg;
        i++;
    }
    threatCountinList = i;
        
}

template<class DataAnalysisClass>
void JSONLoggerDelegate<DataAnalysisClass>::CWTrackConversion
(
            DataAnalysisClass &analysisObj ,  uint64_t currentIteration
)
{

}

template<class DataAnalysisClass>
void JSONLoggerDelegate<DataAnalysisClass>::tracksConversionToJSON()
{
    lengthofString = 0;
    lengthofString+=sprintf(buffer.begin(),"{[");
    for(int i=0; i<threatCountinList;i++)
    {
        if(i!=(threatCountinList-1))
            lengthofString += sprintf(buffer.begin() + lengthofString, "{\"TOAFirst\":%lld,\"TOALast\":%lld \"Band\":\"%s\",\"AOA\":%lld, \"PW\":%lld,\"PRI\":%lld, \"RF\":%lld},",
                                      listOfreportedThreats[i].currentSystemIteration,
                                      listOfreportedThreats[i].TOAFirst,
                                      listOfreportedThreats[i].TOALast,
                                      listOfreportedThreats[i].Band,
                                      listOfreportedThreats[i].AOA,
                                      listOfreportedThreats[i].PW,
                                      listOfreportedThreats[i].PRI,
                                      listOfreportedThreats[i].RF

            );
        else
            lengthofString += sprintf(buffer.begin() + lengthofString, "{\"TOAFirst\":%lld,\"TOALast\":%lld \"Band\":\"%s\",\"AOA\":%lld, \"PW\":%lld,\"PRI\":%lld, \"RF\":%lld}]}",
                                      listOfreportedThreats[i].currentSystemIteration,
                                      listOfreportedThreats[i].TOAFirst,
                                      listOfreportedThreats[i].TOALast,
                                      listOfreportedThreats[i].Band,
                                      listOfreportedThreats[i].AOA,
                                      listOfreportedThreats[i].PW,
                                      listOfreportedThreats[i].PRI,
                                      listOfreportedThreats[i].RF);
    }

}

template<class DataAnalysisClass>
void JSONLoggerDelegate<DataAnalysisClass>::clearBuffer()
{
    listOfreportedThreats = {};
    lengthofString  = 0;
}

template<class DataAnalysisClass>
char * JSONLoggerDelegate<DataAnalysisClass>::getBuffer() 
{
    return buffer.data();
}

template<class DataAnalysisClass>
inline  size_t JSONLoggerDelegate<DataAnalysisClass>::getBufferSize()
{
    return lengthofString;
}