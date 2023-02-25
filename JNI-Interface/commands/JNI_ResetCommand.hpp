#pragma once

#include "JNI_Command.hpp"

class JNI_ResetCommand final:public JNI_Command{
public:
    JNI_ResetCommand(std::shared_ptr<CatoptricController> surfaceIn,Type typeIn):JNI_Command(surfaceIn,typeIn){};
    void execute(std::vector<int>);
};