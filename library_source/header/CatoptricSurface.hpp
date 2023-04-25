#pragma once

#include <cstring>

#include "CatoptricRow.hpp"
#include "SerialFSM.hpp"
#include <thread>
#include <future>
#define STR_EQ 0
#define UNDEF_ORDER -4

#define NO_DEVICES 512
#define SERIAL_NUM_LEN 20  /* Number characters in Arduino serial number */
#define NUM_ROWS 32
#define CONTROLLER_RUNNING 1
#define CMD_LEN 100
#define SYSTEM_SUCCESS 0
#define CMP_EQUAL 0
#define SETUP_SLEEP_TIME 2
#define RUN_SLEEP_TIME 1   /* Sleep secs (int) per 
                              CatopricSurface::run() cycle */

// Typically defaulted to in functional prototype (e.g. 2x2 array)
#define DEFAULT_ROW_LEN 2

#define LS_ID_FILENAME ".serialInfo"  // Will be created if absent
#define LS_WC_FILENAME ".numFiles"    // Will be created if absent
#define LS_CSV_FILENAME ".csvSearch"  // Will be created if absent
#define ARDUINO_IDS_MAP_FILENAME "ids_map"          // Must be present in dir
#define DIMENSIONS_FILENAME "surface_dimensions"    // Must be present in dir
#define SERIAL_INFO_PREFIX_MACRO "usb-Arduino__www.arduino.cc__0043_"
#define DEVICE_PATH "/dev/serial/by-id"

#define PROGRESS_BAR_LEN 68 // Number of characters long in progress bar

struct SerialPort {
    std::string serialNumber;   // Serial number (how to obtain in C++?)
    int row;            // Row of Arduino according to serialPortOrder
    std::string device; // Path to port (symlink in /dev/serial/by-id)

    SerialPort();
    SerialPort(std::string serialIn, int rowIn);
    SerialPort(std::string serialIn, int rowIn, std::string deviceIn);
};

class SerialPortDict {
    
    private:
        std::vector<SerialPort> dict;

    public:
        std::string getSerialNumber(int row);
        int getRow(std::string serialNumber);
        void addPort(SerialPort port);
};

/* Encodes dimensions of surface in the form of map from row number to 
 * row length.
 */
class SurfaceDimensions {

    private:
        // Number of mirrors in a row, doesn't account for alignment nor spacing
        std::vector<int> rowLengths;

    public:
        
        int initDimensions(std::string filePath);
        int getLength(unsigned rowNumber);
};

class CatoptricSurface {

    private:
        // Thread for running the surface
        std::future<void> future;
        int numRowsConnected;
        // Number of rows in oded dictionary of the setup's Arduinos and each serial number
        SerialPortDict serialPortOrder; 
        // Encodes the dimensions of surface (length of each row)
        SurfaceDimensions dimensions;
        // Vector of objects, each representing one row in surface
        std::vector<CatoptricRow> rowInterfaces;
        // Vector of objects each representing a serial port open to an Arduino
        std::vector<SerialPort> serialPorts;
        // Vector of cells read in from the latest CSV
        std::vector<std::string> csvData;
        // Prefix for VFS entry of a serial port connected to an Arduino
        std::string SERIAL_INFO_PREFIX; 

        std::vector<SerialPort> getOrderedSerialPorts();
        std::vector<SerialPort> readSerialPorts(std::string baseDir);
        void setupRowInterfaces();
        void getCSV(std::string path);
        std::vector<std::string> getNextLineAndSplitIntoTokens(
                std::istream& str);
        void parseCSVLine(int csvLineInd, int& rowRead, 
                int& mirrorColumn, int& motorNumber, int& position);
        int initSerialPortOrder(std::string portsMapFile);
        void drawProgressBar(int total, int ackd);
    public:
        CatoptricSurface();
        void update();
        void reset(bool test);
        void updateByCSV(std::string path);
        void cleanup();
        void moveMirror(const int, const int, const int, const int,const int);
};

