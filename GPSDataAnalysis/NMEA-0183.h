// Analysis GPS NMEA-0183 Protocol
#pragma once
#ifndef NMEA-0183
#include "stdafx.h"

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
    // $GPGGA, 102006.60, 3105.56366, N, 12131.98347, E, 1, 06, 1.56, 8.4, M, 10.0, M, , *50
    const string ProtocolCommand = "GPGGA";
    const string ProtocolNameCN = "GPS定位信息";
    const string ProtocolNameEN = "Global Positioning System Fix Data";
    string UTCtime;              // (1) UTC时间                   hhmmss.ss
    string latitude;               // (2) 纬度                          ddmm.mmmmm
    string NorS;                   // (3) 纬度半球                    N/S
    string longitude;            // (4) 经度                          dddmm.mmmmm
    string EorW;                   // (5) 经度半球                   E/W
    string GPSstatus;            // (6) GPS状态                   0:未定位 /1:非差分定位 /2: 差分定位
    string SatellitesUsing;     // (7) 定位卫星数量             00~12
    string HDOP;                  // (8) 水平精确度因子          0.5~99.9
    string altitude;               // (9) 海拔高度                    -9999.9~9999.9
    string GeoidAltitude;      // (10) 大地水准面高度        -9999.9~9999.9
    string DifferentalTime;   // (11) 差分时间                  秒数
    string DifferentalRef;     // (12) 差分参考基站标号      0000~1023
};

class NMEA0183
{

public:
    void SetFrame(string __GPSDataFrame);
    void Refresh();
private:
    string GPSDataFrame;
    GPGGA a;
    
};
#endif // !NMEA-0183
