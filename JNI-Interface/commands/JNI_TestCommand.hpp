#pragma once
#include "JNI_Command.hpp"

class JNI_TestCommand final:public JNI_Command{
public:
    JNI_TestCommand(std::shared_ptr<CatoptricController> surfaceIn,Type typeIn):JNI_Command(surfaceIn,typeIn){};
    void execute(std::vector<int>);
};