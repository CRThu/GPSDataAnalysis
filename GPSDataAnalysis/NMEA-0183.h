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
    // $GPGGA,        (1)     ,        (2)      ,(3),        (4)        ,(5),(6),(7), (8) , (9)  , M, (10) ,M,(11),(12)*hh
    // $GPGGA, 102006.60, 3105.56366, N, 12131.98347, E,  1, 06, 1.56, 8.4, M, 10.0,M,      ,      *50
    const string ProtocolCommand = "GPGGA";
    const string ProtocolNameCN = "GPS定位信息";
    const string ProtocolNameEN = "Global Positioning System Fix Data";
    string UTCtime;              // (1) UTC时间                   hhmmss.ss
    string latitude;               // (2) 纬度                          ddmm.mmmmm
    string NorS;                   // (3) 纬度半球                    N/S
    string longitude;            // (4) 经度                          dddmm.mmmmm
    string EorW;                   // (5) 经度半球                   E/W
    string GPSstatus;            // (6) GPS状态                   0: 未定位 /1: 非差分定位 /2: 差分定位
    string SatellitesNum;      // (7) 定位卫星数量             00~12
    string HDOP;                  // (8) 水平精确度因子          0.5~99.9
    string altitude;               // (9) 海拔高度                    -9999.9~9999.9
    string GeoidAltitude;      // (10) 大地水准面高度        -9999.9~9999.9
    string DifferentalTime;   // (11) 差分时间                  秒数
    string DifferentalRef;     // (12) 差分参考基站标号      0000~1023
};

struct GPGSA
{
    // $GPGSA,(1),(2),        {(3.0),(3.1),...,(3.11)}       ,  (4)  ,  (5) ,  (6)   * hh
    // $GPGSA, A,  3 , 24, 18, 20, 10, 21, 15, , , , , , , 3.39, 1.56, 3.00 * 01
    const string ProtocolCommand = "GPGSA";
    const string ProtocolNameCN = "当前卫星信息";
    const string ProtocolNameEN = "GPS DOP and Active Satellites";
    string status;                       // (1) 模式                  M: 手动 /A: 自动
    string LocationType;            // (2) 定位类型           1: 未定位 /2: 2D定位 /3: 3D定位
    string SatellitesName[12];    // (3) 定位卫星号        01~32
    string PDOP;                        // (4) 综合精度因子     0.5~99.9
    string HDOP;                       // (5) 水平精度因子     0.5~99.9
    string VDOP;                       // (6) 垂直精度因子      0.5~99.9
};

struct GPGSV
{
    //    $GPGSV,(1),(2), (3),{(4), (5), (6) ,(7)},{(4), (5), (6) , (7)},{(4), (5), (6) ,(7)},{(4), (5), (6) ,(7)}*hh
    //    $GPGSV, 3 , 1 , 10, 05, 07, 123,     , 10, 24, 315, 22, 12, 17, 142, 16, 13, 14, 060,     *7C
    //    $GPGSV, 3 , 2 , 10, 15, 44, 048, 23, 18, 55, 323, 27, 20, 52, 118, 28, 21, 49, 243, 32 * 73
    //    $GPGSV, 3 , 3 , 10, 24, 81, 079, 25, 32, 07, 267, *7B
    const string ProtocolCommand = "GPGSV";
    const string ProtocolNameCN = "可见卫星数";
    const string ProtocolNameEN = "GPS Satellites in View";
    string GSVall;                      // (1) GSV语句总数
    string GSVnow;                   // (2) 本句GSV编号
    string SeeSatellitesNum;     // (3) 可见卫星总数
    vector<SatellitesInViewData> SatellitesData;
    //string Name;                    // (4) 卫星编号
    //string AngleOfElevation;   // (5) 卫星仰角                00~90
    //string AngleOfAzimuth;    // (6) 卫星方位角             000~359
    //string SNR;                       // (7) 信噪比                   00~99
};

struct SatellitesInViewData
{
    string Name;                    // (4) 卫星编号
    string AngleOfElevation;   // (5) 卫星仰角                00~90
    string AngleOfAzimuth;    // (6) 卫星方位角             000~359
    string SNR;                       // (7) 信噪比                   00~99
};

struct GPRMC
{
    //    $GPRMC,      (1)      ,(2),        (3)      ,(4),        (5)        ,(6),   (7)   ,    (8)   ,    (9)    ,(10),(11),(12)* hh
    //    $GPRMC, 102006.60, A, 3105.56366, N, 12131.98347, E, 21.014, 254.28, 071017,     ,     , A   * 54
    const string ProtocolCommand = "GPRMC";
    const string ProtocolNameCN = "推荐定位信息";
    const string ProtocolNameEN = "Recommended Minimum Specific GPS/TRANSIT Data";
    string UTCtime;                      // (1) UTC时间                  hhmmss.ss
    string status;                          // (2) 定位状态                  A: 有效定位 /V: 无效定位
    string latitude;                       // (3) 纬度                         ddmm.mmmmm
    string NorS;                           // (4) 纬度半球                   N/S
    string longitude;                    // (5) 经度                         dddmm.mmmmm
    string EorW;                          // (6) 经度半球                   E/W 
    string speed;                         // (7) 地面速率                   000.0~999.9 节
    string direction;                     // (8) 地面航向                   000.0~359.9 度
    string UTCdate;                     // (9) UTC日期                   ddmmyy
    string MagneticDeclination;  // (10) 磁偏角                     000.0~180.0 度
    string MagneticDirection;      // (11) 磁偏角方向              E/W
    string model;                         // (12) 模式指示                 A: 自主定位 /D: 差分定位 /E: 估算 /N: 数据无效
};

struct GPVTG
{
    //    $GPVTG,    (1)   , T,(2),M,    (3)   , N,     (4)   , K,(5)* hh
    //    $GPVTG, 254.28, T,   , M, 21.014, N, 38.917, K, A * 36
    const string ProtocolCommand = "GPVTG";
    const string ProtocolNameCN = "地面速度信息";
    const string ProtocolNameEN = "Track Make Good and Ground Speed";
    string North;                   // (1) 正北基准航向       000~359 度
    string MagneticNorth;    // (2) 磁北基准航向        000~359 度
    string SpeedKnot;           // (3) 地面速率(节)        000.0~999.9 节
    string SpeedKm;             // (4) 地面速率(公里)     000.0~1851.8 公里/小时
    string status;                  // (5) 模式                    A: 自主定位 /D: 差分定位 /E: 估算 /N: 数据无效
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
