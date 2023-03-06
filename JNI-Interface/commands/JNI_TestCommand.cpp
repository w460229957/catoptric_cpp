#include "JNI_TestCommand.hpp"
#include <iostream>

JNI_TestCommand::JNI_TestCommand(std::shared_ptr<CatoptricController> ControllerIn):JNI_Command(ControllerIn,JNI_Command::Type::TEST){
    std::cout << "Test Command Created" << std::endl;
};


void JNI_TestCommand::execute(std::vector<int> args){
    std::cout << "Test Command Executed" << std::endl;
};