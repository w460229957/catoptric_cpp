#include "JNI_service_impTest.hpp"
#include "../commands/JNI_MoveCommand.hpp"
#include "../commands/JNI_TestCommand.hpp"
#include "../commands/JNI_ResetCommand.hpp"
#include "../commands/JNI_QuitCommand.hpp"
#include "../../CatoptricController.hpp"
#include <initializer_list>

JNI_service_impTest::JNI_service_impTest():controller{new CatoptricController()}{
    availCommands[JNI_Command::Type::MOVE] = std::make_unique<JNI_MoveCommand>(controller,JNI_Command::Type::MOVE);
    availCommands[JNI_Command::Type::TEST] = std::make_unique<JNI_TestCommand>(controller,JNI_Command::Type::TEST);
    availCommands[JNI_Command::Type::RESET] = std::make_unique<JNI_ResetCommand>(controller,JNI_Command::Type::RESET);
    availCommands[JNI_Command::Type::QUIT] = std::make_unique<JNI_QuitCommand>(controller,JNI_Command::Type::QUIT);
}


void JNI_service_impTest::move(std::vector<int> input_list){
    availCommands[JNI_Command::Type::MOVE]->execute(input_list);
    return;
}

//Reset the surface to the default position.That is, all mirrors will do some movement and then move back to the default position.
void JNI_service_impTest::reset(){
    availCommands[JNI_Command::Type::RESET]->execute({});
    return;
}

//Test a specific mirror to see if it is working.
//Like how reset() works, but this command will not let the mirror move back to the default position.
void JNI_service_impTest::test(){
    availCommands[JNI_Command::Type::TEST]->execute({});
    return;
}

//Be honest, I don't know what this command is for yet.
void JNI_service_impTest::quit(){
    availCommands[JNI_Command::Type::QUIT]->execute({});
    return;
}