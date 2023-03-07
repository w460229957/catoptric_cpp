#include "JNI_TestCommand.hpp"
#include <iostream>

JNI_TestCommand::JNI_TestCommand(std::shared_ptr<CatoptricSurface> SurfaceIn):JNI_Command(SurfaceIn,JNI_Command::Type::TEST){
    std::cout << "Test Command Created" << std::endl;
}
void JNI_TestCommand::execute(const std::initializer_list<int> & input_list){
    std::cout << "Test Command Executed" << std::endl;
    surface->reset(TEST_RESET);
};