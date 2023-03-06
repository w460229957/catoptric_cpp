#include "JNI_ResetCommand.hpp"
#include <iostream>

JNI_ResetCommand::JNI_ResetCommand(std::shared_ptr<CatoptricController> ControllerIn):JNI_Command(ControllerIn,JNI_Command::Type::RESET){
    std::cout <<"JNI_ResetCommand Called"<<std::endl;
};


void JNI_ResetCommand::execute(std::vector<int> input_list){
    std::cout <<"JNI_ResetCommand::execute()"<<std::endl;
    return;
}
