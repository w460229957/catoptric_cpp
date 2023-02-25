#include "JNI_QuitCommand.hpp"
#include <iostream>

/***
 * @brief This is the constructor of the class JNI_QuitCommand.
 * @details This constructor is used to initialize the data member of the quit command.
 * @param surfaceIn The pointer to the surface that is controlled by an instance of JNI_service.
 * @param typeIn The type of the command.
 * @return None.
*/

JNI_QuitCommand::JNI_QuitCommand(std::shared_ptr<CatoptricController> surfaceIn,Type typeIn):JNI_Command(surfaceIn,typeIn){
    std::cout <<"JNI_QuitCommand::JNI_QuitCommand()"<<std::endl;
    return;
}

void JNI_QuitCommand::execute(std::vector<int> input_list){
    std::cout <<"JNI_QuitCommand::execute()"<<std::endl;
    return;
}

