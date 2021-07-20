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

#define STR_EQUAL 0
#define DIR_LEN_INIT -10

#define TEST_RESET    1
#define NO_TEST_RESET 0

#define ARCHIVE_DIR "csv/archive"
#define NEW_CSV_DIR "csv/new"

class CatoptricController {

    private:
        CatoptricSurface surface;

        void checkForNewCSV();
        std::string getUserInput(std::string inputMessage);
        int getNumFiles(std::string dir);
        int extractFirstIntFromFile(std::istream& filStream);
        int renameMoveFile(std::string src, std::string dest);
        std::string extractName(std::string userInput);

        std::vector<std::string> newCSVs;

    public:
        CatoptricController();
        void run();
};
