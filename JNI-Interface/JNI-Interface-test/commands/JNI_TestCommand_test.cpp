#include "../../commands/JNI_TestCommand.hpp"


int main(){
    JNI_TestCommand testCommand(nullptr);
    testCommand.execute({});
    return 0;
}