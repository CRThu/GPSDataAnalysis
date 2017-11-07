// Analysis GPS NMEA-0183 Protocol
#include "stdafx.h"
#include "NMEA-0183.h"
using namespace std;

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
    unsigned int GetChecksum = 0;
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
    return GetChecksum == CalcChecksum ? 0 : -1;
}

int NMEA0183::GPGLLRefresh()
{
    int QueneEnd;
    string QueneData;
    int QueneCnt = 1;
    unsigned int GetChecksum = 0;
    unsigned int CalcChecksum = 0;
    bool CheckByte = false;
    for (int i = 0; i < GPGLLDataFrame.RawFrame.length(); i++)
    {
        if (GPGLLDataFrame.RawFrame[i] == '*')
            CheckByte = false;
        if (CheckByte)
            CalcChecksum ^= GPGLLDataFrame.RawFrame[i];
        if (GPGLLDataFrame.RawFrame[i] == '$')
        {
            CheckByte = true;
            CalcChecksum = 0;
        }
        if (GPGLLDataFrame.RawFrame[i] == ',' || GPGLLDataFrame.RawFrame[i] == '*')     // DataBegin
        {
            for (int j = i + 1; j < GPGLLDataFrame.RawFrame.length()
                && GPGLLDataFrame.RawFrame[j] != ','
                && GPGLLDataFrame.RawFrame[j] != '*'; j++)    // DataEnd
                QueneEnd = j;
            QueneData = GPGLLDataFrame.RawFrame.substr(i + 1, (QueneEnd - i) < 0 ? 0 : QueneEnd - i);

            switch (QueneCnt)   // Transfer to struct
            {
            case 1:     // (1)
                GPGLLDataFrame.latitude = QueneData;
                break;
            case 2:     // (2)
                GPGLLDataFrame.NorS = QueneData;
                break;
            case 3:     // (3)
                GPGLLDataFrame.longitude = QueneData;
                break;
            case 4:     // (4)
                GPGLLDataFrame.EorW = QueneData;
                break;
            case 5:     // (5)
                GPGLLDataFrame.UTCtime = QueneData;
                break;
            case 6:     // (6)
                GPGLLDataFrame.status = QueneData;
                break;
            case 7:     // (7)
                GPGLLDataFrame.model = QueneData;
                break;
            case 8:    // checksum
                GetChecksum = GetCheckSum(QueneData);
#if DEBUG_CALC == 1
                cout << "GPS.GPGLLRefresh().GetChecksum : " << GetChecksum << endl;
                cout << "GPS.GPGLLRefresh().CalcChecksum : " << CalcChecksum << endl;
#endif
                break;
            }
            QueneCnt++;
        }
    }
    return GetChecksum == CalcChecksum ? 0 : -1;
}

int NMEA0183::GPGSARefresh()
{
    int QueneEnd;
    string QueneData;
    int QueneCnt = 1;
    unsigned int GetChecksum = 0;
    unsigned int CalcChecksum = 0;
    bool CheckByte = false;
    for (int i = 0; i < GPGSADataFrame.RawFrame.length(); i++)
    {
        if (GPGSADataFrame.RawFrame[i] == '*')
            CheckByte = false;
        if (CheckByte)
            CalcChecksum ^= GPGSADataFrame.RawFrame[i];
        if (GPGSADataFrame.RawFrame[i] == '$')
        {
            CheckByte = true;
            CalcChecksum = 0;
        }
        if (GPGSADataFrame.RawFrame[i] == ',' || GPGSADataFrame.RawFrame[i] == '*')     // DataBegin
        {
            for (int j = i + 1; j < GPGSADataFrame.RawFrame.length()
                && GPGSADataFrame.RawFrame[j] != ','
                && GPGSADataFrame.RawFrame[j] != '*'; j++)    // DataEnd
                QueneEnd = j;
            QueneData = GPGSADataFrame.RawFrame.substr(i + 1, (QueneEnd - i)<0 ? 0 : QueneEnd - i);

            switch (QueneCnt)   // Transfer to struct
            {
            case 1:     // (1)
                GPGSADataFrame.status = QueneData;
                break;
            case 2:     // (2)
                GPGSADataFrame.LocationType = QueneData;
                break;
            case 3:     case 4:     case 5:     case 6: 
            case 7:     case 8:     case 9:     case 10:
            case 11:   case 12:    case 13:   case 14:    // {(3.0)~(3.11)}
                GPGSADataFrame.SatellitesName[QueneCnt - 3] = QueneData;
                break;
            case 15:    // (4)
                GPGSADataFrame.PDOP = QueneData;
                break;
            case 16:    // (5)
                GPGSADataFrame.HDOP = QueneData;
                break;
            case 17:    // (6)
                GPGSADataFrame.VDOP = QueneData;
                break;
            case 18:    // checksum
                GetChecksum = GetCheckSum(QueneData);
#if DEBUG_CALC == 1
                cout << "GPS.GPGSARefresh().GetChecksum : " << GetChecksum << endl;
                cout << "GPS.GPGSARefresh().CalcChecksum : " << CalcChecksum << endl;
#endif
                break;
            }
            QueneCnt++;
        }
    }
    return GetChecksum == CalcChecksum ? 0 : -1;
}

int NMEA0183::GPGSVRefresh()
{
    SatellitesInViewData SatTemp;
    int ChecksumFlag = 0;
    for (int n = 0; n < GPGSVDataFrame.RawFrame.size(); n++)
    {
        int QueneEnd;
        string QueneData;
        int QueneCnt = 1;
        unsigned int GetChecksum = 0;
        unsigned int CalcChecksum = 0;
        bool CheckByte = false;
        for (int i = 0; i < GPGSVDataFrame.RawFrame[n].length(); i++)
        {
            if (GPGSVDataFrame.RawFrame[n][i] == '*')
                CheckByte = false;
            if (CheckByte)
                CalcChecksum ^= GPGSVDataFrame.RawFrame[n][i];
            if (GPGSVDataFrame.RawFrame[n][i] == '$')
            {
                CheckByte = true;
                CalcChecksum = 0;
            }
            if (GPGSVDataFrame.RawFrame[n][i] == ',' || GPGSVDataFrame.RawFrame[n][i] == '*')     // DataBegin
            {
                for (int j = i + 1; j < GPGSVDataFrame.RawFrame[n].length()
                    && GPGSVDataFrame.RawFrame[n][j] != ','
                    && GPGSVDataFrame.RawFrame[n][j] != '*'; j++)    // DataEnd
                    QueneEnd = j;
                QueneData = GPGSVDataFrame.RawFrame[n].substr(i + 1, (QueneEnd - i) < 0 ? 0 : QueneEnd - i);

                if (GPGSVDataFrame.RawFrame[n][i] == '*')
                {
                    GetChecksum = GetCheckSum(QueneData);
#if DEBUG_CALC == 1
                    cout << "GPS.GPGSVRefresh().GetChecksum : " << GetChecksum << endl;
                    cout << "GPS.GPGSVRefresh().CalcChecksum : " << CalcChecksum << endl;
#endif
                }

                switch (QueneCnt)   // Transfer to struct
                {
                case 1:     // (1)
                    break;
                case 2:     // (2)
                    break;
                case 3:     // (3)
                    GPGSVDataFrame.SeeSatellitesNum = QueneData;
                    break;
                case 4:          case 5:         case 6:           case 7:
                case 8:          case 9:         case 10:         case 11:
                case 12:        case 13:       case 14:         case 15:
                case 16:        case 17:       case 18:         case 19:
                    if (QueneCnt % 4 == 0)
                        SatTemp.Name = QueneData;
                    if (QueneCnt % 4 == 1)
                        SatTemp.AngleOfElevation = QueneData;
                    if (QueneCnt % 4 == 2)
                        SatTemp.AngleOfAzimuth = QueneData;
                    if (QueneCnt % 4 == 3)
                    {
                        SatTemp.SNR = QueneData;
                        GPGSVDataFrame.SatellitesData.push_back(SatTemp);
                    }
                    break;
                }
                QueneCnt++;
            }
        }
        ChecksumFlag += (GetChecksum == CalcChecksum ? 0 : -1);
    }
    return ChecksumFlag == 0 ? 0 : -1;
}

int NMEA0183::GPRMCRefresh()
{
    int QueneEnd;
    string QueneData;
    int QueneCnt = 1;
    unsigned int GetChecksum = 0;
    unsigned int CalcChecksum = 0;
    bool CheckByte = false;
    for (int i = 0; i < GPRMCDataFrame.RawFrame.length(); i++)
    {
        if (GPRMCDataFrame.RawFrame[i] == '*')
            CheckByte = false;
        if (CheckByte)
            CalcChecksum ^= GPRMCDataFrame.RawFrame[i];
        if (GPRMCDataFrame.RawFrame[i] == '$')
        {
            CheckByte = true;
            CalcChecksum = 0;
        }
        if (GPRMCDataFrame.RawFrame[i] == ',' || GPRMCDataFrame.RawFrame[i] == '*')     // DataBegin
        {
            for (int j = i + 1; j < GPRMCDataFrame.RawFrame.length()
                && GPRMCDataFrame.RawFrame[j] != ','
                && GPRMCDataFrame.RawFrame[j] != '*'; j++)    // DataEnd
                QueneEnd = j;
            QueneData = GPRMCDataFrame.RawFrame.substr(i + 1, (QueneEnd - i)<0 ? 0 : QueneEnd - i);

            switch (QueneCnt)   // Transfer to struct
            {
            case 1:     // (1)
                GPRMCDataFrame.UTCtime = QueneData;
                break;
            case 2:     // (2)
                GPRMCDataFrame.status = QueneData;
                break;
            case 3:     // (3)
                GPRMCDataFrame.latitude = QueneData;
                break;
            case 4:     // (4)
                GPRMCDataFrame.NorS = QueneData;
                break;
            case 5:     // (5)
                GPRMCDataFrame.longitude = QueneData;
                break;
            case 6:     // (6)
                GPRMCDataFrame.EorW = QueneData;
                break;
            case 7:     // (7)
                GPRMCDataFrame.speed = QueneData;
                break;
            case 8:     // (8)
                GPRMCDataFrame.direction = QueneData;
                break;
            case 9:     // (9)
                GPRMCDataFrame.UTCdate = QueneData;
                break;
            case 10:     // (10)
                GPRMCDataFrame.MagneticDeclination = QueneData;
                break;
            case 11:     // (11)
                GPRMCDataFrame.MagneticDirection = QueneData;
                break;
            case 12:     // (12)
                GPRMCDataFrame.model = QueneData;
                break;
            case 13:    // checksum
                GetChecksum = GetCheckSum(QueneData);
#if DEBUG_CALC == 1
                cout << "GPS.GPRMCRefresh().GetChecksum : " << GetChecksum << endl;
                cout << "GPS.GPRMCRefresh().CalcChecksum : " << CalcChecksum << endl;
#endif
                break;
            }
            QueneCnt++;
        }
    }
    return GetChecksum == CalcChecksum ? 0 : -1;
}

int NMEA0183::GPVTGRefresh()
{
    int QueneEnd;
    string QueneData;
    int QueneCnt = 1;
    unsigned int GetChecksum = 0;
    unsigned int CalcChecksum = 0;
    bool CheckByte = false;
    for (int i = 0; i < GPVTGDataFrame.RawFrame.length(); i++)
    {
        if (GPVTGDataFrame.RawFrame[i] == '*')
            CheckByte = false;
        if (CheckByte)
            CalcChecksum ^= GPVTGDataFrame.RawFrame[i];
        if (GPVTGDataFrame.RawFrame[i] == '$')
        {
            CheckByte = true;
            CalcChecksum = 0;
        }
        if (GPVTGDataFrame.RawFrame[i] == ',' || GPVTGDataFrame.RawFrame[i] == '*')     // DataBegin
        {
            for (int j = i + 1; j < GPVTGDataFrame.RawFrame.length()
                && GPVTGDataFrame.RawFrame[j] != ','
                && GPVTGDataFrame.RawFrame[j] != '*'; j++)    // DataEnd
                QueneEnd = j;
            QueneData = GPVTGDataFrame.RawFrame.substr(i + 1, (QueneEnd - i) < 0 ? 0 : QueneEnd - i);

            switch (QueneCnt)   // Transfer to struct
            {
            case 1:     // (1)
                GPVTGDataFrame.North = QueneData;
                break;
            case 2:     // T
                    break;
            case 3:     // (2)
                GPVTGDataFrame.MagneticNorth = QueneData;
                break;
            case 4:     // M
                break;
            case 5:     // (3)
                GPVTGDataFrame.SpeedKnot = QueneData;
                break;
            case 6:     // N
                break;
            case 7:     // (4)
                GPVTGDataFrame.SpeedKm = QueneData;
                break;
            case 8:     // K
                break;
            case 9:     // (5)
                GPVTGDataFrame.model = QueneData;
                break;
            case 10:    // checksum
                GetChecksum = GetCheckSum(QueneData);
#if DEBUG_CALC == 1
                cout << "GPS.GPVTGRefresh().GetChecksum : " << GetChecksum << endl;
                cout << "GPS.GPVTGRefresh().CalcChecksum : " << CalcChecksum << endl;
#endif
                break;
            }
            QueneCnt++;
        }
    }
    return GetChecksum == CalcChecksum ? 0 : -1;
}

