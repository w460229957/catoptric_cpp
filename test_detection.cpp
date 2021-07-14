
#include <vector>
#include <fstream>
#include <cstdio>
#include <cstring>
#include "CatoptricSurface.hpp"

#define ERR_QUERY_FAILED -9

using namespace std;

SerialPort::SerialPort() {
    serialNumber = string();
    row = UNDEF_ORDER;
}

SerialPort::SerialPort(string numIn, int rowIn, string deviceIn) {
    serialNumber = numIn;
    row = rowIn;
    device = deviceIn;
}

SerialPort::SerialPort(string numIn, int rowIn) {
    serialNumber = numIn;
    row = rowIn;
    device = string();
}

vector<SerialPort> CatoptricSurface::readSerialPorts() {

    vector<SerialPort> serialPorts;

    fstream serialInfoFile;
    serialInfoFile.open(LS_ID_FILENAME, ios::in);
    if(serialInfoFile.is_open()) {
        string serialInfoLine;
        while(getline(serialInfoFile, serialInfoLine)) {
            string serialNumber;
            
            /* Format of Arduino Uno's file name in /dev/serial/by-id is
             *     usb-Arduino__www.arduino.cc__0043_XXXXXXXXXXXXXXXXXXX-YYYY
             * for serial number X's (SERIAL_INFO_PREFIX contains the prefix)
             */

            if(serialInfoLine.find(SERIAL_INFO_PREFIX) != string::npos) {
                serialNumber = serialInfoLine.substr(SERIAL_INFO_PREFIX.size(),
                        SERIAL_NUM_LEN);
            }

            int row = serialPortOrder.getRow(serialNumber);
            if(row == ERR_QUERY_FAILED) {
                printf("Unrecognized device: serialNumber %s\n", 
                        serialNumber.c_str());
            } else {
                serialPorts.push_back(SerialPort(serialNumber, row, 
                            serialInfoLine));
            }
        }

    }
    serialInfoFile.close();

    return serialPorts;
}

