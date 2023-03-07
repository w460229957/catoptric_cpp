#pragma once
#include "../../CatoptricSurface.hpp"
#include <memory>

class JNI_Command{
protected:
    std::shared_ptr<CatoptricSurface> surface;//One raspberry pi can only control one surface.
public:
    enum class Type{MOVE,RESET,TEST,QUIT};
    Type commandType;
    JNI_Command(std::shared_ptr<CatoptricSurface> surface,Type commandType):surface(surface),commandType(commandType){};
    virtual void execute(const std::initializer_list<int> &) = 0;
};