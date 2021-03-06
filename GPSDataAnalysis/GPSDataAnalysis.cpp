// GPSDataAnalysis.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "NMEA-0183.h"

int main()
{
    system("CHCP 936");
    string data = "$GPRMC, 232530.00, A, 3359.08811, N, 11850.91461, E, 430.275, 7.74, 071017, , , A * 62 \
        $GPVTG, 7.74, T, , M, 430.275, N, 796.869, K, A * 31 \
        $GPGGA, 232530.00, 3359.08811, N, 11850.91461, E, 1, 09, 1.06, 8622.3, M, 1.8, M, , *59 \
        $GPGSA, A, 3, 01, 11, 22, 03, 08, 17, 07, 28, 30, , , , 2.23, 1.06, 1.97*0B \
        $GPGSV, 3, 1, 11, 01, 61, 040, 37, 03, 36, 132, 20, 06, 07, 225, , 07, 19, 194, 20 * 7C \
        $GPGSV, 3, 2, 11, 08, 15, 078, 32, 11, 46, 046, 35, 17, 37, 295, 17, 19, 17, 279, *74 \
        $GPGSV, 3, 3, 11, 22, 40, 100, 35, 28, 69, 329, 27, 30, 37, 225, 20 * 42 \
        $GPGLL, 3359.08811, N, 11850.91461, E, 232530.00, A, A * 66";
    NMEA0183 GPS;
    GPS.CleanFrame();
        cout << "return GPS.SetFrame(data) : " << GPS.SetFrame(data) << endl;
        cout << "return GPS.GPGGARefresh() : " << GPS.GPGGARefresh()<<endl;
        cout << "return GPS.GPGSARefresh() : " << GPS.GPGSARefresh() << endl;
        cout << "return GPS.GPGSVRefresh() : " << GPS.GPGSVRefresh() << endl;
        cout << "return GPS.GPRMCRefresh() : " << GPS.GPRMCRefresh() << endl;
        cout << "return GPS.GPGLLRefresh() : " << GPS.GPGLLRefresh() << endl;
        cout << "return GPS.GPVTGRefresh() : " << GPS.GPVTGRefresh() << endl;
    cout << GPS.GPGGADataFrame.ProtocolNameCN;
    getchar();
    return 0;
}

