// Analysis GPS NMEA-0183 Protocol
#include "stdafx.h"
#include "NMEA-0183.h"

void NMEA0183::CleanFrame()
{
    GPGGADataFrame.RawFrame = "";
    GPGLLDataFrame.RawFrame = "";
    GPGSADataFrame.RawFrame = "";
    GPGSVDataFrame.RawFrame = "";
    GPRMCDataFrame.RawFrame = "";
    GPVTGDataFrame.RawFrame = "";
}

void NMEA0183::TrimFrame(string &s)
{
    int index = 0;
    if (!s.empty())
        while ((index = s.find(' ', index)) != string::npos)
            s.erase(index, 1);
}

void NMEA0183::SetFrame(string __GPSDataFrame)
{
    TrimFrame(__GPSDataFrame);
    int ProtocolBegin=0;
    int ProtocolEnd = 0;
    string ProtocolStr;
    string RawFrame;

    for (int i=0;i<__GPSDataFrame.length();i++)
    {
        if (__GPSDataFrame[i] == '$')
        {
            ProtocolStr = __GPSDataFrame.substr(i + 1, 5);    // i+1 ~ i+6
            //cout << ProtocolStr<<endl;
            ProtocolBegin = i;
        }
        if (__GPSDataFrame[i] == '*')   // *hh
        {
            ProtocolEnd = i + 2;
            RawFrame = __GPSDataFrame.substr(ProtocolBegin, (ProtocolEnd - ProtocolBegin + 1));
            //cout << RawFrame << endl;
        }
    }
}
