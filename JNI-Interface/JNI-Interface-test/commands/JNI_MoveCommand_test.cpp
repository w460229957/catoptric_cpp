#include "../../commands/JNI_MoveCommand.hpp"
#include <iostream>

int main(){      
    std::cout << "Testing JNI_MoveCommand" << std::endl;                                           
    JNI_MoveCommand moveCommand(nullptr);
    moveCommand.execute({1,2,3,4,5,6,7,8,9,10});
    return 0;
}