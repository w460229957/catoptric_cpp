#pragma once
#include "JNI_Command.hpp"


#ifndef TEST_RESET
#define TEST_RESET 1
#endif

class JNI_TestCommand final:public JNI_Command{
public:
    JNI_TestCommand(std::shared_ptr<CatoptricSurface> ControllerIn);
    void execute(const std::initializer_list<int> &);
};