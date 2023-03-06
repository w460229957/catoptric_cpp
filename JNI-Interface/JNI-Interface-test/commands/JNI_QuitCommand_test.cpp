#include "../../commands/JNI_QuitCommand.hpp"
#include <iostream>

int main(){
    std::cout << "Testing JNI_QuitCommand" << std::endl;
    JNI_QuitCommand quitCommand(nullptr);
    quitCommand.execute({});
    return 0;
}
