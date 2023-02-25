#include "../../CatoptricController.hpp"
#include <jni.h>
#include <memory>

class JNI_Command{
protected:
    std::shared_ptr<CatoptricController> controller;//One raspberry pi can only control one surface.
public:
    enum class Type{MOVE,RESET,TEST,QUIT};
    Type Command_type;
    JNI_Command(std::shared_ptr<CatoptricController> surfaceIn,Type typeIn):controller(surfaceIn),Command_type(typeIn){};
    virtual void execute(std::vector<int>) = 0;
};