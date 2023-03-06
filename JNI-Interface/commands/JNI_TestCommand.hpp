#pragma once
#include "JNI_Command.hpp"

class JNI_TestCommand final:public JNI_Command{
public:
    JNI_TestCommand(std::shared_ptr<CatoptricController> ControllerIn);
    void execute(std::vector<int>);
};