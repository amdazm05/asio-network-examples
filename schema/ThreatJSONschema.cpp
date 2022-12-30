#include "ThreatJSONschema.hpp"

template<class DataAnalysisClass,class TrackDefaultClass>
void JSONLoggerDelegate<DataAnalysisClass,TrackDefaultClass>::PulsedTrackConversion
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

template<class DataAnalysisClass, class TrackDefaultClass>
void JSONLoggerDelegate<DataAnalysisClass,TrackDefaultClass>::CWTrackConversion
(
            std::vector<TrackDefaultClass> &tracks ,  uint64_t currentIteration
)
{
    for (uint32_t i = 0; i < tracks.size(); i++)
	{
		        TrackDefaultClass &track = tracks[i];
	            listOfreportedCWThreats[i].Angle=track.analysisObj.angle.curAngle;
				listOfreportedCWThreats[i].Quad=track.angle.curQuadrant;
                listOfreportedCWThreats[i].heading = track.power.angle.bearingDisplay;
                listOfreportedCWThreats[i].cwGainStage = track.power.cwGainStage;
                listOfreportedCWThreats[i].tA= track.power.avgPA;
                listOfreportedCWThreats[i].tb= track.power.avgPB;
                listOfreportedCWThreats[i].Band = "CW";
                listOfreportedCWThreats[i].tc= track.power.avgPC;
                listOfreportedCWThreats[i].td= track.power.avgPD;
				listOfreportedCWThreats[i].RF=track.rf.accumulateRf[0].rfAvg;
    }
    CWthreatCountinList = tracks.size();
}

template<class DataAnalysisClass,class TrackDefaultClass>
void JSONLoggerDelegate<DataAnalysisClass,TrackDefaultClass>::tracksConversionToJSON()
{
    lengthofString = 0;
    lengthofString+=sprintf(buffer.begin(),"\"{ PulsedTracks : \"{[");
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
            lengthofString += sprintf(buffer.begin() + lengthofString, "{\"TOAFirst\":%lld,\"TOALast\":%lld \"Band\":\"%s\",\"AOA\":%lld, \"PW\":%lld,\"PRI\":%lld, \"RF\":%lld}]",
                                      listOfreportedThreats[i].currentSystemIteration,
                                      listOfreportedThreats[i].TOAFirst,
                                      listOfreportedThreats[i].TOALast,
                                      listOfreportedThreats[i].Band,
                                      listOfreportedThreats[i].AOA,
                                      listOfreportedThreats[i].PW,
                                      listOfreportedThreats[i].PRI,
                                      listOfreportedThreats[i].RF);
    }

    lengthofString+=sprintf(buffer.begin(),"\"{ PulsedTracks : \"{[");
    for(int i=0; i<CWthreatCountinList;i++)
    {
        if(i!=(CWthreatCountinList-1))
            lengthofString += sprintf(buffer.begin() + lengthofString, "{\"Band\":%s,\"Angle\":%lld \"Quad\":\"%lld\",\"Heading\":%lld, \"RF\":%lld,\"GS\":%lld, \"pA\":%lld, \"pB\":%lld, \"pC\":%lld,\"pD\":%lld},",
                                    listOfreportedCWThreats[i].Band,
                                    listOfreportedCWThreats[i].Angle,
                                    listOfreportedCWThreats[i].Quad,
                                    listOfreportedCWThreats[i].heading, 
                                    listOfreportedCWThreats[i].RF
                                    listOfreportedCWThreats[i].cwGainStage,
                                    listOfreportedCWThreats[i].tA,
                                    listOfreportedCWThreats[i].tb,
                                    listOfreportedCWThreats[i].tc,
                                    listOfreportedCWThreats[i].td,

            );
        else
            lengthofString += sprintf(buffer.begin() + lengthofString, "{\"Band\":%s,\"Angle\":%lld \"Quad\":\"%lld\",\"Heading\":%lld, \"RF\":%lld,\"GS\":%lld, \"pA\":%lld, \"pB\":%lld, \"pC\":%lld,\"pD\":%lld}]",
                                    listOfreportedCWThreats[i].Band,
                                    listOfreportedCWThreats[i].Angle,
                                    listOfreportedCWThreats[i].Quad,
                                    listOfreportedCWThreats[i].heading, 
                                    listOfreportedCWThreats[i].RF
                                    listOfreportedCWThreats[i].cwGainStage,
                                    listOfreportedCWThreats[i].tA,
                                    listOfreportedCWThreats[i].tb,
                                    listOfreportedCWThreats[i].tc,
                                    listOfreportedCWThreats[i].td,
            );
    }

}

template<class DataAnalysisClass,class TrackDefaultClass>
void JSONLoggerDelegate<DataAnalysisClass,TrackDefaultClass>::clearBuffer()
{
    listOfreportedThreats = {};
    lengthofString  = 0;
    threatCountinList = 0;
    CWthreatCountinList = 0;
}

template<class DataAnalysisClass,class TrackDefaultClass>
char * JSONLoggerDelegate<DataAnalysisClass,TrackDefaultClass>::getBuffer() 
{
    return buffer.data();
}

template<class DataAnalysisClass, class TrackDefaultClass>
inline  size_t JSONLoggerDelegate<DataAnalysisClass, TrackDefaultClass>::getBufferSize()
{
    return lengthofString;
}