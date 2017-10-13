// Analysis GPS NMEA-0183 Protocol
#include "stdafx.h"
#include "NMEA-0183.h"

void NMEA0183::CleanFrame()
{
    GPGGADataFrame.RawFrame = "";
    GPGLLDataFrame.RawFrame = "";
    GPGSADataFrame.RawFrame = "";
    GPGSVDataFrame.RawFrame.swap(vector<string>());
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

int NMEA0183::SetFrame(string __GPSDataFrame)
{
    TrimFrame(__GPSDataFrame);
    int ProtocolBegin=0;
    int ProtocolEnd = 0;
    int ProtocolCnt = 0;
    string ProtocolStr;
    string RawFrame;

    for (int i=0;i<__GPSDataFrame.length();i++)
    {
        if (__GPSDataFrame[i] == '$')   // $    FrameBegin
        {
            ProtocolStr = __GPSDataFrame.substr(i + 1, 5);    // i+1 ~ i+6
#if DEBUG_CALC == 1
            cout << "SetFrame()::ProtocolStr : " << ProtocolStr << endl;
#endif
            ProtocolBegin = i;
        }
        if (__GPSDataFrame[i] == '*')   // *hh  FrameEnd
        {
            ProtocolEnd = i + 2;
            RawFrame = __GPSDataFrame.substr(ProtocolBegin, (ProtocolEnd - ProtocolBegin + 1));
#if DEBUG_CALC == 1
            cout << "SetFrame()::RawFrame : " << RawFrame << endl;
#endif
            // Transfer Frame
            ProtocolCnt++;
            if (ProtocolStr == GPGGADataFrame.ProtocolCommand)
                GPGGADataFrame.RawFrame = RawFrame;
            else if (ProtocolStr == GPGLLDataFrame.ProtocolCommand)
                GPGLLDataFrame.RawFrame = RawFrame;
            else if (ProtocolStr == GPGSADataFrame.ProtocolCommand)
                GPGSADataFrame.RawFrame = RawFrame;
            else if (ProtocolStr == GPGSVDataFrame.ProtocolCommand)
                GPGSVDataFrame.RawFrame.push_back(RawFrame);
            else if (ProtocolStr == GPRMCDataFrame.ProtocolCommand)
                GPRMCDataFrame.RawFrame = RawFrame;
            else if (ProtocolStr == GPVTGDataFrame.ProtocolCommand)
                GPVTGDataFrame.RawFrame = RawFrame;
        }
    }
    return ProtocolCnt;
}

int NMEA0183::GPGGARefresh()
{
    // TODO
    return 0;
}
