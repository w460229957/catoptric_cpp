#include "JNI_TestCommand.hpp"
#include <iostream>

JNI_TestCommand::JNI_TestCommand(std::shared_ptr<CatoptricController> surfaceIn,Type typeIn):JNI_Command(surfaceIn,typeIn){
    std::cout << "Test Command Created" << std::endl;
};


void JNI_TestCommand::execute(std::vector<int> args){
    std::cout << "Test Command Executed" << std::endl;
}