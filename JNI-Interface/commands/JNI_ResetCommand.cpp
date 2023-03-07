#include "JNI_ResetCommand.hpp"
#include <iostream>

JNI_ResetCommand::JNI_ResetCommand(std::shared_ptr<CatoptricSurface> SurfaceIn):JNI_Command(SurfaceIn,JNI_Command::Type::RESET){
    std::cout <<"JNI_ResetCommand Called"<<std::endl;
};


void JNI_ResetCommand::execute(const std::initializer_list<int> & input_list){
    std::cout <<"JNI_ResetCommand::execute()"<<std::endl;
    
    surface->reset(REGULAR_RESET);
    return;
}
