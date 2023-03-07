#pragma once
#include "JNI_Command.hpp"

class JNI_MoveCommand final:public JNI_Command{
public:
    JNI_MoveCommand(std::shared_ptr<CatoptricSurface> ControllerIn);
    void execute(const std::initializer_list<int> &);
};