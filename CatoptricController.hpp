/* CatoptricController.hpp
 * The CatoptricController class defines the event loop for the overall
 * catoptric lighting system and input processing.
 * Author: Scott Sirri scottsirri@wustl.edu
 */

#pragma once

#include <vector>
#include <cstring>
#include "CatoptricSurface.hpp"
#include "ErrCodes.hpp"
#include <memory>
#define STR_EQUAL 0
#define DIR_LEN_INIT -10

#define TEST_RESET    1
#define REGULAR_RESET 0

#define ARCHIVE_DIR "csv/archive"   // Where CSVs are archived after execution
#define NEW_CSV_DIR "csv/new"       // Where new CSVs are deposited

class CatoptricController {

    private:
        std::shared_ptr<CatoptricSurface> surface;
        std::vector<std::string> newCSVs;

        void checkForNewCSV();
        std::string getUserInput(std::string inputMessage);
        int getNumFiles(std::string dir);
        int extractFirstIntFromFile(std::istream& filStream);
        int renameMoveFile(std::string src, std::string dest);
        std::string extractName(std::string userInput);
        void findCSV(int& csvInd, std::string& csvName, std::string targetName);
        void archiveCSV(std::string csvPath, std::string csvName);
        void receiveUserInput();
    public:
        void moveMirror(const int, const int, const int, const int,const int);
        CatoptricController();
        void run();
};
