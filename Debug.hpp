#pragma once
/***
*@file Debug.hpp
*@brief Debugging function and macros
*@details This file contains macros and functions for debugging.
*@author ZhengYuan Zhang
*/


#ifdef DEBUG
int Debug(const char* messageToWrite, const char * debugFilePath="debug.txt");
#endif


