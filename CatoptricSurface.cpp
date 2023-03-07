
#include <algorithm> // sort
#include <stdlib.h>
#include <cstdio>      // snprintf
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <errno.h> 
    
#include "CatoptricSurface.hpp"
#include "ErrCodes.hpp"

using namespace std;

/* Each row reads incoming data and updates its SerialFSM object, sends queued
 * message to its Arduino.
 * Sleep and print update message.
 */
void CatoptricSurface::run() {

    printf("\n\n");
    int commandsOut = 1, updates = 0;

    while(commandsOut > 0) {

        commandsOut = 0;
        /* Each row reads incoming data and updates SerialFSM objects, 
           sends messages from the back of respective commandQueue */
        for(CatoptricRow& cr : rowInterfaces) cr.update();

        int commandsQueue = 0, ackCount = 0, nackCount = 0;
        
        for(CatoptricRow& cr : rowInterfaces) {
            commandsOut += cr.fsmCommandsOut();
            ackCount += cr.fsmAckCount();
            nackCount += cr.fsmNackCount();
            commandsQueue += cr.commandQueue.size();
        }

        updates++;
        sleep(RUN_SLEEP_TIME);
        // 'commands in queue' is nonzero only when too many commands are
        // pending and no more can be sent
        printf("\r%2d commands out | %d commands in queue | %2d acks | "
               "%d nacks | %d cycles\n", commandsOut, commandsQueue, ackCount, 
                nackCount, updates);
        drawProgressBar(commandsOut + ackCount, ackCount);
        printf("\033[F"); // Moves stdout cursor up one line
    }

    printf("\n\n");

    for(CatoptricRow& cr : rowInterfaces) {
        cr.fsm.ackCount = 0;
        cr.fsm.nackCount = 0;
    }
}

CatoptricSurface::CatoptricSurface() {

    SERIAL_INFO_PREFIX = SERIAL_INFO_PREFIX_MACRO;

    initSerialPortOrder(ARDUINO_IDS_MAP_FILENAME);

    serialPorts = getOrderedSerialPorts();
    numRowsConnected = serialPorts.size();
    
    dimensions.initDimensions(DIMENSIONS_FILENAME);
    setupRowInterfaces();
    
    sleep(SETUP_SLEEP_TIME);

    setbuf(stdout, NULL);
    run();
}

/* Reads a config file to populate the map of Arduino USB ids to row numbers.
 * Each line in the config file is a serial number followed by a space and
 * then the respective row number.
 */
int CatoptricSurface::initSerialPortOrder(string portsMapFilename) {

    fstream portsMapStream;
    portsMapStream.open(portsMapFilename.c_str(), ios::in);
    string line, arduino_id_str, row_str;
    int row_int;
    if(portsMapStream.is_open()) {
        while(getline(portsMapStream, line)) {
            istringstream iss(line);
            iss >> arduino_id_str;
            iss >> row_str;
            try {
                row_int = stoi(row_str);
            } catch(...) {
                printf("Ill-formatted row number");
                return ERR_STOI;
            }

            serialPortOrder.addPort(
                    SerialPort(arduino_id_str.c_str(), row_int));
        }

        portsMapStream.close();
    }
    
    return RET_SUCCESS;
}

/* Returns a vector of SerialPort objects each representing a connected Arduino,
 * sorted by serial number. 
 */
vector<SerialPort> CatoptricSurface::getOrderedSerialPorts() {

    string path = DEVICE_PATH;
    string ls_id_cmd = "ls " + path + " > " + LS_ID_FILENAME;

    int ret = system(ls_id_cmd.c_str());
    if(ret == NO_DEVICES) {
        printf("No devices detected in \'%s\'\n", DEVICE_PATH);
        return vector<SerialPort>();
    }

    // Read VFS entries for serial ports from 'ls' output
    vector<SerialPort> serialPorts = readSerialPorts(path);


    struct serialComp {
        // Used to sort serial ports by their Arduino's serial number
        bool operator() (SerialPort a, SerialPort b) {
            return a.serialNumber.compare(b.serialNumber) < 0;
        }
    } serialCompObj;

    // Sort serial ports by their serial number
    sort(serialPorts.begin(), serialPorts.end(), serialCompObj);

    for(SerialPort sp : serialPorts) {
        printf("Arduino #%s : Row #%d\n", sp.serialNumber.c_str(), sp.row);
    }

    return serialPorts;
}

/* Read the file containing 'ls' output to scan VFS entries.
 * Serial ports reside in the base directory passed to the function.
 * Return vector of SerialPort objects representing only connected Arduinos.
 */
vector<SerialPort> CatoptricSurface::readSerialPorts(string baseDir) {

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

                string path;
                if(baseDir.empty()) path = "./" + serialInfoLine;
                else path = baseDir + "/" + serialInfoLine;
                
                printf(" *** Detected serial port %s ***\n", path.c_str());

                serialPorts.push_back(SerialPort(serialNumber, row, path));
            }
        }

        serialInfoFile.close();
    }

    return serialPorts;
}

/* Initializes a CatoptricRow object for each available Arduino.
 * Populates rowInterfaces accordingly.
 */
void CatoptricSurface::setupRowInterfaces() {
    for(SerialPort sp : serialPorts) {

        string port = sp.device;
        char cstr[port.size()];
        strcpy(cstr, port.c_str());

        int rowNum = sp.row;
        int rowLen = dimensions.getLength(rowNum);

		printf(" -- Initializing Catoptric Row %d with %d mirrors\n", 
                rowNum, rowLen);

        CatoptricRow cr(rowNum, rowLen, cstr);
	    rowInterfaces.push_back(cr);
    }
}

/* Reset the orientation of every mirror or test functionality of the 
 * motors/connection (depending on 'test' parameter) and resume running.
 */
void CatoptricSurface::reset(bool test) {
    if(!test) printf(" -- Resetting all mirrors to default position\n");
    else printf(" -- Testing all motors\n");

    for(CatoptricRow& cr : rowInterfaces) {
        cr.reset(test); // Reset whole row
    }

    run();
}

/* Clear the old cached CSV data.
 * Read the new passed CSV file and insert cells into csvData (omit delimiters).
 */
void CatoptricSurface::getCSV(string path) {

    csvData.clear(); // Clear old, cached CSV data

    bool readData = false;
    ifstream fs(path.c_str());
    while(fs && !fs.eof()) {
        readData = true;
        // Get vector of next line's elements
        vector<string> nextLinesCells = getNextLineAndSplitIntoTokens(fs);
        // Append the next line's cells onto csvData
        csvData.insert(csvData.end(), nextLinesCells.begin(), 
                nextLinesCells.end());
   }

   if(!readData) printf("Didn't read data from CSV %s\n", path.c_str());

   fs.close();
}

/* Returns a vector of all cells in the next unread line from the CSV
 * corresponding to the passed i(f)stream.
 */
vector<string> CatoptricSurface::getNextLineAndSplitIntoTokens(istream& strm) {
    
    vector<string> result;
    string line, cell;
    
    getline(strm, line);
    stringstream lineStream(line);

    while(getline(lineStream, cell, ',')) {
        result.push_back(cell);
    }

    if(result.size() == 0 && line.size() > 0) {
        printf("Failed to read data from CSV line [%s] in "
                "getNextLineAndSplitIntoTokens\n", line.c_str());
    }

    return result;
}

/* Read passed CSV.
 * Push a sequence of Message objects onto the commandQueue according to the
 * contents of the CSV.
 * Resume running.
 */
void CatoptricSurface::updateByCSV(string path) {

    getCSV(path);   // Read in CSV contents to csvData

    for(CatoptricRow& cr : rowInterfaces) {
        cr.resetSerialBuffer();
    }

    // Synthesize a Message object per CSV line (four cells per line)
    for(unsigned csvLineInd = 0; csvLineInd < csvData.size(); csvLineInd += 4) {
        
        int rowRead, mirrorColumn, motorNumber, position;
        parseCSVLine(csvLineInd, rowRead, mirrorColumn, motorNumber, position);

        bool foundRow = false;
        Message msg(rowRead, mirrorColumn, motorNumber, position);

        /* Remember that row numbers are 1-indexed in the protocol, but
           0-indexed in these data structures! */
        for(int rowInd = 0; rowInd < NUM_ROWS; ++rowInd) {
            if(rowRead == rowInterfaces[rowInd].getRowNumber()) {
                foundRow = true;
                rowInterfaces[rowRead - 1].reorientMirrorAxis(msg);
                break;
            }
        }

        if(!foundRow) {
            printf("Line %d of CSV is addressed to a row that does not exist"
                    ": %d (protocol is 1-indexed!)", csvLineInd, rowRead);
        }
    }

    run();
}

/* Assign fields for new Message based on specified line from csvData.
 */
void CatoptricSurface::parseCSVLine(int csvLineInd, int& rowRead, 
        int& mirrorColumn, int& motorNumber, int& position) {
    try {
        rowRead      = stoi(csvData[csvLineInd]);
        mirrorColumn = stoi(csvData[csvLineInd + 1]);
        motorNumber  = stoi(csvData[csvLineInd + 2]);
        position     = stoi(csvData[csvLineInd + 3]);
    } catch(...) {
        printf("Invalid CSV data passed to stoi in updateByCSV: %s\n", 
                strerror(errno));
        return;
    }
}

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

/* Returns the name of the port with the passed row number,
 * if any. Returns empty string otherwise.
 */
string SerialPortDict::getSerialNumber(int row) {
    for(unsigned i = 0; i < dict.size(); ++i) {
        if(dict[i].row == row) return dict[i].serialNumber;
    }

    return string();
}

/* Returns the row number for the serial port associated
 * with the passed name. Returns ERR_QUERY_FAILED otherwise.
 */
int SerialPortDict::getRow(string serialNumber) {
    for(unsigned i = 0; i < dict.size(); ++i) {
        if(dict[i].serialNumber.compare(serialNumber) == STR_EQ) {
            return dict[i].row;
        }
    }

    return ERR_QUERY_FAILED;
}

/* Adds a new serial port object the the CatoptricSurface object's
 * serial port dictionary.
 */
void SerialPortDict::addPort(SerialPort port) {
    dict.push_back(port);
}

/* Reads file containing surface dimensions and initializes the corresponding
 * vector with row lengths.
 * A file containing surface dimensions (row lengths) is formatted as intervals
 * of row numbers, each row in an interval having the same length (conducive to
 * rectangular-ish arrays).
 *      Two integers (separated by a space) per line.
 *      First integer is the last (one-indexed) row number in that interval.
 *          The first row number in that interval is implicitly one greater than
 *          the end of the previous interval (first interval starts at 1).
 *          Both bounds are INCLUSIVE.
 *      Second integer is the length of each row.
 */
int SurfaceDimensions::initDimensions(string filePath) {

    ifstream dimensionsFile(filePath);

    if(dimensionsFile) {

        string dimensionsLine;
        int rowNumStart = 1, rowNumEnd, rowLen;

        while(getline(dimensionsFile, dimensionsLine)) {
            
            istringstream iss(dimensionsLine);

            try {
                iss >> rowNumEnd;
                iss >> rowLen;
            } catch(...) {
                printf("istringstream error- invalid dimensions file contents: "
                        "%s\n", strerror(errno));
                return ERR_DIMS_FILE;
            }

            // Requires space linear in # rows, but easy indexing operation
            for(int i = 0; i < rowNumEnd - rowNumStart + 1; ++i) {
                rowLengths.push_back(rowLen);
            }

            rowNumStart = rowNumEnd + 1;
        }
    }

    dimensionsFile.close();

    return RET_SUCCESS;
}

/* Returns the length of the row with the passed row number.
 * Row numbers are ONE-INDEXED! (not my fault)
 */
int SurfaceDimensions::getLength(unsigned rowNumber) {
    if(1 <= rowNumber && rowNumber <= rowLengths.size()) {
        return rowLengths[rowNumber - 1];
    } else {
        return DEFAULT_ROW_LEN;
    }
}

void CatoptricSurface::cleanup() {
    for(CatoptricRow& cr : rowInterfaces) cr.cleanup();
}

/***
 * @date  2023-03-04
 * @author ZhengYuan Zhang
 * @brief  Reorients the mirror axis of the specified row to the specified
 *        position. 
 * @param  msg  Message object containing the row number and position to
 *       reorient the mirror axis to.
 * @return  void
*/
void CatoptricSurface::moveMirror(const int &rowNum, const int &mirrorID, const int &whichMotor, const int & directionOfTheMotor,const int &steps){
    try{
        rowInterfaces[rowNum-1].stepMotor(mirrorID, whichMotor, directionOfTheMotor, steps);
    }
    catch(...){
        printf("Error in moving mirror");
        throw runtime_error("Error in moving mirror");
    }
}

void CatoptricSurface::drawProgressBar(int total, int ackd) {
    
    int numCharsPerMsg = PROGRESS_BAR_LEN / total;
    if(numCharsPerMsg <= 0) numCharsPerMsg = 1;

    for(int i = 0; i < ackd; ++i) {
        for(int j = 0; j < numCharsPerMsg; ++j) printf("|");
    }
    for(int i = 0; i < total - ackd; ++i) {
        for(int j = 0; j < numCharsPerMsg; ++j) printf("-");
    }
    printf("]");
}