#pragma once
#include "JNI_Command.hpp"

class JNI_QuitCommand final:public JNI_Command{
public:
    JNI_QuitCommand(std::shared_ptr<CatoptricController> surfaceIn,Type typeIn):JNI_Command(surfaceIn,typeIn){};
    void execute(std::vector<int>);
};