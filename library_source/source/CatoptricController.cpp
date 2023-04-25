
/* CatoptricController.cpp
 * The CatoptricController class defines the event loop for the overall
 * catoptric lighting system and input processing.
 * Author: Scott Sirri scottsirri@wustl.edu
 */

#include <condition_variable>
#include <fstream>   // ifstream
#include <iostream>  // cin
#include <sstream>   // stringstream
#include <algorithm> // transform
#include "../header/CatoptricController.hpp"
using namespace std;

void CatoptricController::moveMirror(const int rowNum, const int mirrorID, const int whichMotor, const int directionOfTheMotor, const int steps)
{
    cout << "moving mirror- "
         << "rowNum " << rowNum << " "
         << "mirror(col) " << mirrorID << " "
         << "whichMotor " << whichMotor << " "
         << "directionOfTheMother " << directionOfTheMotor << " "
         << "steps " << steps << endl;
    surface->moveMirror(rowNum, mirrorID, whichMotor, directionOfTheMotor, steps);
}

CatoptricController::CatoptricController() : surface{new CatoptricSurface()}
{
    // Initialize one thread for each surface
    std::cout << "CatoptricController initialized" << std::endl;
}

/* Repeatedly check for new CSV files and prompt user for input:
 * either reset the mirrors or execute a new CSV file.
 */

void CatoptricController::run(int argc, char *argv[])
{

    while (CONTROLLER_RUNNING)
    { // Infinite event loop

        // Retrieve any new CSV files
        checkForNewCSV();
        printf("\n-------------------------\n\n");
        string inputMsg = "\'Reset\' mirrors, 'Test' motors, or 'Check' for a"
                          " new file to run: ";

        if (newCSVs.size() > 0)
        {
            inputMsg = "\'Reset\' mirrors, 'Test' motors, or \'Run\' file: \'Move\' to move a single mirror\n";
        }

        string userInput = getUserInput(inputMsg); // Converts to lowercase

        if (userInput.compare("quit") == STR_EQUAL)
        {
            surface->cleanup(); // Free resources
            return;
        }
        else if (userInput.compare("check") == STR_EQUAL)
        {
            checkForNewCSV();
        }
        else if (userInput.compare("reset") == STR_EQUAL)
        {
            surface->reset(REGULAR_RESET);
            printf(" -- Reset Complete\n");
        }
        else if (userInput.compare("test") == STR_EQUAL)
        {
            surface->reset(TEST_RESET);
            printf("\tBEWARE: Orientation of mirrors is no longer "
                   "guaranteed!\n");
            printf(" -- Test Complete\n");
        }
        else if (newCSVs.size() > 0 &&
                 userInput.substr(0, 3).compare("run") == STR_EQUAL)
        {

            string providedName = extractName(userInput);
            // Arbitrary default choice if providedName isn't found
            int csvInd = 0;
            string csvName = newCSVs[csvInd];

            // If providedName is in newCSVs, set csvInd and csv accordingly
            findCSV(csvInd, csvName, providedName);

            // Removes the csvInd'th element
            newCSVs.erase(next(newCSVs.begin(), csvInd));

            printf(" -- Running \'%s\'\n", csvName.c_str());

            // NEW_CSV_DIR is the dir for CSVs that haven't been executed
            string csvPath = NEW_CSV_DIR "/" + csvName;

            // Update mirrors' orientations based on CSV contents
            surface->updateByCSV(csvPath);
            printf(" -- \'%s\' ran successfully\n", csvName.c_str());

            // Renames & archives the csv
            archiveCSV(csvPath, csvName);
        }
        else if (userInput == "move")
        {
            receiveUserInput();
        }
        else
        {
            printf(" -- Invalid input\n");
        }
    }
}

void CatoptricController::receiveUserInput()
{
    int rowNum, mirrorID, whichMotor, directionOfTheMotor, steps;
    std::cout << "Enter row number: ";
    std::cin >> rowNum;
    std::cout << "Enter mirror ID: ";
    std::cin >> mirrorID;
    std::cout << "Enter which motor: ";
    std::cin >> whichMotor;
    std::cout << "Enter direction of the motor: ";
    std::cin >> directionOfTheMotor;
    std::cout << "Enter steps: ";
    std::cin >> steps;
    moveMirror(rowNum, mirrorID, whichMotor, directionOfTheMotor, steps);
}
/* Renames and moves the passed file to the archive, appending a prefix to the
 * filename based on how many files are already archived.
 */
void CatoptricController::archiveCSV(string csvPath, string csv)
{

    // The archive directory sometimes mysteriously disappears?
    system("mkdir -p csv/archive");

    // Find the number of files in csv/archive
    string archiveDir = ARCHIVE_DIR;
    int archiveLength = getNumFiles(archiveDir);
    if (archiveLength < 0)
    {
        printf("Error in getNumFiles\n");
        return;
    }

    // Rename + move CSV file to csv/archive
    string newName = archiveDir + "/" + to_string(archiveLength) +
                     "_" + csv;
    if (renameMoveFile(csvPath, newName) < 0)
    {
        printf("Error in renameMoveFile\n");
        return;
    }

    printf(" -- \'%s\' moved to archive\n", csv.c_str());
}

/* Seeks the passed CSV in newCSVs and updates csvInd & csvName accordingly if
 * it's found.
 */
void CatoptricController::findCSV(
    int &csvInd, string &csvName, string targetName)
{

    if (!targetName.empty())
    {
        for (unsigned i = 0; i < newCSVs.size(); ++i)
        {
            if (newCSVs[i].compare(targetName) == STR_EQUAL)
            {
                csvInd = i;
                csvName = targetName;
                break;
            }
        }
    }
}

/* Takes a filename. Examines extension to see if it's a .csv file
 */
bool isCSV(string name)
{
    string csvEnding = ".csv";
    if (name.compare(name.length() - csvEnding.length(),
                     csvEnding.length(), csvEnding) == CMP_EQUAL)
    {
        return true;
    }
    else
        return false;
}

/* Check csv/new directory for any newly deposited CSVs.
 * Uses 'system' function to execute command-line instructions for filesystem
 * inspection.
 */
void CatoptricController::checkForNewCSV()
{

    // Create file listing contents of csv/new
    string directoryStr = NEW_CSV_DIR;
    string ls_cmd = "ls " + directoryStr + " > " + LS_CSV_FILENAME;
    int ret;
    if ((ret = system(ls_cmd.c_str())) != SYSTEM_SUCCESS)
    {
        printf("System function error: return  %d, %s\n", ret, strerror(errno));
    }

    ifstream ls_file_stream;
    string ls_line;

    ls_file_stream.open(LS_CSV_FILENAME, ios_base::in);

    while (ls_file_stream.good() && !ls_file_stream.eof() &&
           getline(ls_file_stream, ls_line))
    {

        if (isCSV(ls_line))
        {
            bool alreadyTracked = false;

            for (string trackedCSV : newCSVs)
            {
                if (trackedCSV.compare(ls_line) == CMP_EQUAL)
                {
                    alreadyTracked = true;
                }
            }

            if (!alreadyTracked)
            {
                newCSVs.push_back(ls_line);
                printf(" -- Found new csv file \'%s\'\n", ls_line.c_str());
            }
            else
            {
                printf(" -- Available csv file \'%s\'\n", ls_line.c_str());
            }
        }
    }

    return;
}

/* Retrieves user input and returns it lowercase.
 * */
string CatoptricController::getUserInput(string inputMessage)
{
    string userInput;
    std::cout << std::endl;
    printf("%s", inputMessage.c_str());
    getline(cin, userInput);
    printf("\n\n");

    // Transform user input to lowercase characters
    transform(userInput.begin(), userInput.end(),
              userInput.begin(), ::tolower);
    return userInput;
}

/* Return number of files in passed directory path (or error code).
 */
int CatoptricController::getNumFiles(string dir)
{

    int dirLength = DIR_LEN_INIT;
    char ls_wc_cmd[CMD_LEN];
    snprintf(ls_wc_cmd, CMD_LEN, "ls %s | wc > %s", dir.c_str(),
             LS_WC_FILENAME);

    if (system(ls_wc_cmd) != SYSTEM_SUCCESS)
    {
        printf("Error in system function for command \'%s\': %s\n",
               ls_wc_cmd, strerror(errno));
        return ERR_SYSTEM;
    }

    ifstream fs;
    fs.open(LS_WC_FILENAME, ios_base::in);
    if (fs.good() && !fs.eof())
    {
        /* First int in output from 'wc' is number lines, i.e. number
           files listed by ls */
        dirLength = extractFirstIntFromFile(fs);
    }
    else if (fs.fail())
    {
        printf("LS_WC file fail to open: %s\n", strerror(errno));
        return ERR_FAIL_OPEN;
    }

    if (dirLength == ERR_NO_INT || dirLength == ERR_STOI)
    {
        printf("Error finding number files in dir\n");
    }

    return dirLength;
}

/* Extracts the first integer from a file containing only integers (and
 * whitespace).
 * Exclusively employed on the output of 'wc' command to get the number
 * of lines in another file.
 */
int CatoptricController::extractFirstIntFromFile(istream &filStream)
{

    string line;
    getline(filStream, line);

    stringstream lineStream(line);
    string wcElem;

    while (getline(lineStream, wcElem, ' '))
    {
        if (wcElem.empty() != true)
        {
            int stoiRet;
            try
            {
                stoiRet = stoi(wcElem);
            }
            catch (...)
            {
                printf("extractFirstIntFromFile parsing error in stoi: %s\n",
                       strerror(errno));
                stoiRet = ERR_STOI;
            }

            return stoiRet;
        }
    }

    printf("extractFirstIntFromFile error, no int found\n");
    return ERR_NO_INT;
}

/* Renames and moves the passed src file to dest.
 */
int CatoptricController::renameMoveFile(string src, string dest)
{

    string mov_cmd = "mv " + src + " " + dest;
    if (system(mov_cmd.c_str()) != SYSTEM_SUCCESS)
    {
        printf("Error in system function for command \'%s\': %s\n",
               mov_cmd.c_str(), strerror(errno));
        return ERR_SYSTEM;
    }

    return RET_SUCCESS;
}

/* Provided a line of user input beginning with 'run', check for and return
 * any filename following 'run'.
 */
string CatoptricController::extractName(string userInput)
{
    string remaining = userInput.substr(3, userInput.size());
    if (remaining.empty())
        return remaining;

    unsigned left_ind = 0, right_ind = remaining.size() - 1;
    while (left_ind < remaining.size() &&
           (remaining[left_ind] == ' ' || remaining[left_ind] == '\n'))
    {
        ++left_ind;
    }
    while (right_ind > 0 &&
           (remaining[right_ind] == ' ' || remaining[right_ind] == '\n'))
    {
        --right_ind;
    }

    if (left_ind >= right_ind)
        return string();

    string name = remaining.substr(left_ind, right_ind + 1);
    if (name.find(" ") != string::npos)
        return string();

    return name;
}