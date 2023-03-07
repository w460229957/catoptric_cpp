#include "JNI_MoveCommand.hpp"
#include <iostream>
#include <stdexcept>
#include "../../CatoptricRow.hpp"
/***
 * @brief This is the constructor of the class JNI_MoveCommand.
 * @details This constructor is used to initialize the data member of the move command.
 * @param surfaceIn The pointer to the surface that is controlled by an instance of JNI_service.
 * @param typeIn The type of the command.
 * @return None.
*/
JNI_MoveCommand::JNI_MoveCommand(std::shared_ptr<CatoptricSurface> surface):JNI_Command(surface,JNI_Command::Type::MOVE){
    std::cout <<"JNI_MoveCommand::JNI_MoveCommand() called"<<std::endl; 
    return;
    
}

/***
 * @brief This is the function that is used to execute the command.
 * @details This function is used to execute the move command.
 * @param input_list The list of the input parameters.
 * @return None.
*/
void JNI_MoveCommand::execute(const std::initializer_list<int> & input_list){
    std::cout <<"JNI_MoveCommand::execute() called "<<std::endl;
    //Call the moveMirror function of the surface.
    if(input_list.size() < 5){
        throw std::invalid_argument("JNI_MoveCommand::execute() called with invalid input_list->MoveCommand needs at least 5 parameters");
    }
    auto iter = input_list.begin();
    surface->moveMirror(*iter,*(iter+1),*(iter+2),*(iter+3),*(iter+4));
}                                          

