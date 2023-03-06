#include "../../commands/JNI_ResetCommand.hpp"


int main(){
    JNI_ResetCommand resetCommand(nullptr);
    resetCommand.execute({});
    return 0;
}