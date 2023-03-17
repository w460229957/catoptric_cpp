#include "JNI_service_impTest.hpp"
#include "../commands/JNI_MoveCommand.hpp"
#include "../commands/JNI_TestCommand.hpp"
#include "../commands/JNI_ResetCommand.hpp"
#include "../commands/JNI_QuitCommand.hpp"
#include "../../CatoptricSurface.hpp"
#include <initializer_list>
#include <memory>

JNI_service_impTest::JNI_service_impTest():surface{new CatoptricSurface()}{
    availCommands[JNI_Command::Type::MOVE].reset(new JNI_MoveCommand(surface));
    availCommands[JNI_Command::Type::RESET].reset(new JNI_ResetCommand(surface));
    availCommands[JNI_Command::Type::TEST].reset(new JNI_TestCommand(surface));
    availCommands[JNI_Command::Type::QUIT].reset(new JNI_QuitCommand(surface));
    return;
};

void JNI_service_impTest::move(const std::initializer_list<int> & input_list){
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