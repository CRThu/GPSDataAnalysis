// Analysis GPS NMEA-0183 Protocol
#include "NMEA-0183.h"
#include "stdafx.h"

void NMEA0183::SetFrame(string __GPSDataFrame)
{
    // TODO 字符串检测
    DataFrame = __GPSDataFrame;
}
