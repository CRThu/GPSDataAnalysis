// Analysis GPS NMEA-0183 Protocol
#ifndef NMEA-0183
#include "stdafx.h"

#define DEBUG_CALC 1

//    GPS Frame Example : 
//    
//    $GPRMC, 102006.60, A, 3105.56366, N, 12131.98347, E, 21.014, 254.28, 071017, , , A * 54
//    $GPVTG, 254.28, T, , M, 21.014, N, 38.917, K, A * 36
//    $GPGGA, 102006.60, 3105.56366, N, 12131.98347, E, 1, 06, 1.56, 8.4, M, 10.0, M, , *50
//    $GPGSA, A, 3, 24, 18, 20, 10, 21, 15, , , , , , , 3.39, 1.56, 3.00 * 01
//    $GPGSV, 3, 1, 10, 05, 07, 123, , 10, 24, 315, 22, 12, 17, 142, 16, 13, 14, 060, *7C
//    $GPGSV, 3, 2, 10, 15, 44, 048, 23, 18, 55, 323, 27, 20, 52, 118, 28, 21, 49, 243, 32 * 73
//    $GPGSV, 3, 3, 10, 24, 81, 079, 25, 32, 07, 267, *7B
//    $GPGLL, 3105.56366, N, 12131.98347, E, 102006.60, A, A * 6C
//


struct GPGGA
{
    // $GPGGA,        (1)     ,        (2)      ,(3),        (4)        ,(5),(6),(7), (8) , (9)  , M, (10) ,M,(11),(12)*hh
    // $GPGGA, 102006.60, 3105.56366, N, 12131.98347, E,  1, 06, 1.56, 8.4, M, 10.0,M,      ,      *50
    string RawFrame;
    const string ProtocolCommand = "GPGGA";
    const string ProtocolNameCN = "GPS��λ��Ϣ";
    const string ProtocolNameEN = "Global Positioning System Fix Data";
    string UTCtime;              // (1) UTCʱ��                   hhmmss.ss
    string latitude;               // (2) γ��                          ddmm.mmmmm
    string NorS;                   // (3) γ�Ȱ���                    N/S
    string longitude;            // (4) ����                          dddmm.mmmmm
    string EorW;                   // (5) ���Ȱ���                   E/W
    string GPSstatus;            // (6) GPS״̬                   0: δ��λ /1: �ǲ�ֶ�λ /2: ��ֶ�λ
    string SatellitesNum;      // (7) ��λ��������             00~12
    string HDOP;                  // (8) ˮƽ��ȷ������          0.5~99.9
    string altitude;               // (9) ���θ߶�                    -9999.9~9999.9
    string GeoidAltitude;      // (10) ���ˮ׼��߶�        -9999.9~9999.9
    string DifferentalTime;   // (11) ���ʱ��                  ����
    string DifferentalRef;     // (12) ��ֲο���վ���      0000~1023
};

struct GPGLL
{
    //    $GPGLL,        (1)      ,(2),        (3)        ,(4),      (5)      ,(6),(7)* hh
    //    $GPGLL, 3105.56366, N, 12131.98347, E, 102006.60, A, A * 6C
    string RawFrame;
    const string ProtocolCommand = "GPGLL";
    const string ProtocolNameCN = "����λ��Ϣ";
    const string ProtocolNameEN = "Geographic Position";
    string latitude;                       // (1) γ��                         ddmm.mmmmm
    string NorS;                           // (2) γ�Ȱ���                   N/S
    string longitude;                    // (3) ����                         dddmm.mmmmm
    string EorW;                          // (4) ���Ȱ���                   E/W 
    string UTCtime;                     // (5) UTCʱ��                   hhmmss.ss
    string status;                         // (6) ��λ״̬                   A: ��Ч��λ /V: ��Ч��λ
    string model;                        // (7) ģʽָʾ                   A: ������λ /D: ��ֶ�λ /E: ���� /N: ������Ч
};

struct GPGSA
{
    // $GPGSA,(1),(2),        {(3.0),(3.1),...,(3.11)}       ,  (4)  ,  (5) ,  (6)   * hh
    // $GPGSA, A,  3 , 24, 18, 20, 10, 21, 15, , , , , , , 3.39, 1.56, 3.00 * 01
    string RawFrame;
    const string ProtocolCommand = "GPGSA";
    const string ProtocolNameCN = "��ǰ������Ϣ";
    const string ProtocolNameEN = "GPS DOP and Active Satellites";
    string status;                       // (1) ģʽ                  M: �ֶ� /A: �Զ�
    string LocationType;            // (2) ��λ����           1: δ��λ /2: 2D��λ /3: 3D��λ
    string SatellitesName[12];    // (3) ��λ���Ǻ�        01~32
    string PDOP;                        // (4) �ۺϾ�������     0.5~99.9
    string HDOP;                       // (5) ˮƽ��������     0.5~99.9
    string VDOP;                       // (6) ��ֱ��������      0.5~99.9
};

struct SatellitesInViewData
{
    string Name;                    // (4) ���Ǳ��
    string AngleOfElevation;   // (5) ��������                00~90
    string AngleOfAzimuth;    // (6) ���Ƿ�λ��             000~359
    string SNR;                       // (7) �����                   00~99
};

struct GPGSV
{
    //    $GPGSV,(1),(2), (3),{(4), (5), (6) ,(7)},{(4), (5), (6) , (7)},{(4), (5), (6) ,(7)},{(4), (5), (6) ,(7)}*hh
    //    $GPGSV, 3 , 1 , 10, 05, 07, 123,     , 10, 24, 315, 22, 12, 17, 142, 16, 13, 14, 060,     *7C
    //    $GPGSV, 3 , 2 , 10, 15, 44, 048, 23, 18, 55, 323, 27, 20, 52, 118, 28, 21, 49, 243, 32 * 73
    //    $GPGSV, 3 , 3 , 10, 24, 81, 079, 25, 32, 07, 267, *7B
    vector<string> RawFrame;
    const string ProtocolCommand = "GPGSV";
    const string ProtocolNameCN = "�ɼ������� ";
    const string ProtocolNameEN = "GPS Satellites in View";
    string GSVall;                      // (1) GSV�������
    string GSVnow;                   // (2) ����GSV���
    string SeeSatellitesNum;     // (3) �ɼ���������
    vector<SatellitesInViewData> SatellitesData;
    //string Name;                    // (4) ���Ǳ��
    //string AngleOfElevation;   // (5) ��������                00~90
    //string AngleOfAzimuth;    // (6) ���Ƿ�λ��             000~359
    //string SNR;                       // (7) �����                   00~99
};

struct GPRMC
{
    //    $GPRMC,      (1)      ,(2),        (3)      ,(4),        (5)        ,(6),   (7)   ,    (8)   ,    (9)    ,(10),(11),(12)* hh
    //    $GPRMC, 102006.60, A, 3105.56366, N, 12131.98347, E, 21.014, 254.28, 071017,     ,     , A   * 54
    string RawFrame;
    const string ProtocolCommand = "GPRMC";
    const string ProtocolNameCN = "�Ƽ���λ��Ϣ";
    const string ProtocolNameEN = "Recommended Minimum Specific GPS/TRANSIT Data";
    string UTCtime;                      // (1) UTCʱ��                  hhmmss.ss
    string status;                          // (2) ��λ״̬                  A: ��Ч��λ /V: ��Ч��λ
    string latitude;                       // (3) γ��                         ddmm.mmmmm
    string NorS;                           // (4) γ�Ȱ���                   N/S
    string longitude;                    // (5) ����                         dddmm.mmmmm
    string EorW;                          // (6) ���Ȱ���                   E/W 
    string speed;                         // (7) ��������                   000.0~999.9 ��
    string direction;                     // (8) ���溽��                   000.0~359.9 ��
    string UTCdate;                     // (9) UTC����                   ddmmyy
    string MagneticDeclination;  // (10) ��ƫ��                     000.0~180.0 ��
    string MagneticDirection;      // (11) ��ƫ�Ƿ���              E/W
    string model;                         // (12) ģʽָʾ                 A: ������λ /D: ��ֶ�λ /E: ���� /N: ������Ч
};

struct GPVTG
{
    //    $GPVTG,    (1)   , T,(2),M,    (3)   , N,     (4)   , K,(5)* hh
    //    $GPVTG, 254.28, T,   , M, 21.014, N, 38.917, K, A * 36
    string RawFrame;
    const string ProtocolCommand = "GPVTG";
    const string ProtocolNameCN = "�����ٶ���Ϣ";
    const string ProtocolNameEN = "Track Make Good and Ground Speed";
    string North;                   // (1) ������׼����              000~359 ��
    string MagneticNorth;    // (2) �ű���׼����              000~359 ��
    string SpeedKnot;           // (3) ��������(��)               000.0~999.9 ��
    string SpeedKm;             // (4) ��������(����)            000.0~1851.8 ����/Сʱ
    string model;                  // (5) ģʽָʾ                    A: ������λ /D: ��ֶ�λ /E: ���� /N: ������Ч
};


class NMEA0183
{
public:
    // *Frame
    void CleanFrame();
    void TrimFrame(string &s);
    int SetFrame(string __GPSDataFrame);    // return Protocol Lines

    // GPGGA
    GPGGA GPGGADataFrame;
    int GPGGARefresh();
    // GPGLL
    GPGLL GPGLLDataFrame;
    int GPGLLRefresh();
    // GPGSA
    GPGSA GPGSADataFrame;
    int GPGSARefresh();
    // GPGSV
    GPGSV GPGSVDataFrame;
    int GPGSVRefresh();
    // GPRMC
    GPRMC GPRMCDataFrame;
    int GPRMCRefresh();
    // GPVTG
    GPVTG GPVTGDataFrame;
    int GPVTGRefresh();

private:
};

#endif // !NMEA-0183
