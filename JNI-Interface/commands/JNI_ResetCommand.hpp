#pragma once

#include "JNI_Command.hpp"


#ifndef REGULAR_RESET
#define REGULAR_RESET 0
#endif 

class JNI_ResetCommand final:public JNI_Command{
public:
    JNI_ResetCommand(std::shared_ptr<CatoptricSurface> ControllerIn);
    void execute(const std::initializer_list<int> &);
};