#pragma once
#include "JNI_Command.hpp"

class JNI_QuitCommand final:public JNI_Command{
public:
    JNI_QuitCommand(std::shared_ptr<CatoptricSurface> ControllerIn);
    void execute(const std::initializer_list<int> &);
};