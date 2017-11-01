// Analysis GPS NMEA-0183 Protocol
#include "stdafx.h"
#include "NMEA-0183.h"
int HexToInt(char _input)
{
    if (_input >= '0'&&_input <= '9')
        return _input - '0';
    else
        if (_input >= 'a'&&_input <= 'f')
            return _input - 'a' + 10;
        else
            if (_input >= 'A'&&_input <= 'F')
                return _input - 'A' + 10;
            else
                return -1;
}
int GetCheckSum(string _input)
{
    return HexToInt(_input[0]) * 16 + HexToInt(_input[1]);
}

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
    int QueneEnd;
    string QueneData;
    int QueneCnt = 1;
    unsigned int GetChecksum;
    unsigned int CalcChecksum = 0;
    bool CheckByte=false;
    for (int i = 0; i < GPGGADataFrame.RawFrame.length(); i++)
    {
        if (GPGGADataFrame.RawFrame[i] == '*')
            CheckByte = false;
        if (CheckByte)
            CalcChecksum ^= GPGGADataFrame.RawFrame[i];
        if (GPGGADataFrame.RawFrame[i] == '$')
        {
            CheckByte = true;
            CalcChecksum = 0;
        }
        if (GPGGADataFrame.RawFrame[i] == ',' || GPGGADataFrame.RawFrame[i] == '*')     // DataBegin
        {
            for (int j = i+1; j < GPGGADataFrame.RawFrame.length()
                && GPGGADataFrame.RawFrame[j] != ','
                && GPGGADataFrame.RawFrame[j] != '*'; j++)    // DataEnd
                QueneEnd = j;
            QueneData = GPGGADataFrame.RawFrame.substr(i + 1, (QueneEnd - i)<0?0: QueneEnd - i);

            switch (QueneCnt)   // Transfer to struct
            {
            case 1:     // (1)
                GPGGADataFrame.UTCtime = QueneData;
                break;
            case 2:     // (2)
                GPGGADataFrame.latitude = QueneData;
                break;
            case 3:     // (3)
                GPGGADataFrame.NorS = QueneData;
                break;
            case 4:     // (4)
                GPGGADataFrame.longitude = QueneData;
                break;
            case 5:     // (5)
                GPGGADataFrame.EorW = QueneData;
                break;
            case 6:     // (6)
                GPGGADataFrame.GPSstatus = QueneData;
                break;
            case 7:     // (7)
                GPGGADataFrame.SatellitesNum = QueneData;
                break;
            case 8:     // (8)
                GPGGADataFrame.HDOP = QueneData;
                break;
            case 9:     // (9)
                GPGGADataFrame.altitude = QueneData;
                break;
            case 10:    // M
                break;
            case 11:    // (10)
                GPGGADataFrame.GeoidAltitude = QueneData;
                break;
            case 12:    // M
                break;
            case 13:    // (11)
                GPGGADataFrame.DifferentalTime = QueneData;
                break;
            case 14:    // (12)
                GPGGADataFrame.DifferentalRef = QueneData;
                break;
            case 15:    // checksum
                // TODO
                GetChecksum = GetCheckSum(QueneData);
#if DEBUG_CALC == 1
                    cout << "GPS.GPGGARefresh().GetChecksum : " << GetChecksum << endl;
                    cout << "GPS.GPGGARefresh().CalcChecksum : " << CalcChecksum << endl;
#endif
                break;
            }
            QueneCnt++;
        }
    }
    return 0;
}
