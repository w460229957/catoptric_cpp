#include "JNI_MoveCommand.hpp"
#include <iostream>
/***
 * @brief This is the constructor of the class JNI_MoveCommand.
 * @details This constructor is used to initialize the data member of the move command.
 * @param surfaceIn The pointer to the surface that is controlled by an instance of JNI_service.
 * @param typeIn The type of the command.
 * @return None.
*/
JNI_MoveCommand::JNI_MoveCommand(std::shared_ptr<CatoptricController> surfaceIn,Type typeIn):JNI_Command(surfaceIn,typeIn){
    std::cout <<"JNI_MoveCommand::JNI_MoveCommand()"<<std::endl;
    return;
}


/***
 * @brief This is the function that is used to execute the command.
 * @details This function is used to execute the move command.
 * @param input_list The list of the input parameters.
 * @return None.
*/
void JNI_MoveCommand::execute(std::vector<int> input_list){
    std::cout <<"JNI_MoveCommand::execute()"<<std::endl;
    return;
}


