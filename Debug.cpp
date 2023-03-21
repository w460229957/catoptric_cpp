#include "Debug.hpp"
#include <iostream>
#include <fstream>


/***
    * @brief Debugging function
    * @details This function writes or appends a message to a file called "debug.txt",if not specified otherwise.
    * @param messageToWrite The message to write to the file
    * @param debugFilePath The path to the file to write to
    * @return 0 on success, -1 on failure
    * @note This function is only compiled if the DEBUG macro is defined.(See Debug.hpp)
    * @author ZhengYuan Zhang
*/
int Debug(const char* messageToWrite, const char * debugFilePath="debug.txt")
{
    //Create a file in the current directory called "debug.txt"
    //and write the message to it
    std::ofstream debugFile(debugFilePath, std::ios::app | std::ios::out);
    
    if(!debugFile.is_open())
    {
        std::cerr << "Function Debug: Error opening file->" << debugFilePath << std::endl;
        return -1;
    }

    //Get current timestamp
    time_t now = time(0);
    //Convert timestamp to string
    char* dt = ctime(&now);
    //Write the timestamp to the file
    debugFile <<"[ " << dt <<  " ]" <<" : " << messageToWrite << std::endl;
    debugFile.close();
    return 0;
}