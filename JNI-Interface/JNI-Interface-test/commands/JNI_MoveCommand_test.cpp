#include "../commands/JNI_MoveCommand.hpp"
#include "../s"
class ControllerMock : public JNI_Controller{

};
void JNI_MoveCommand_test(){
    JNI_MoveCommand moveCommand;
    moveCommand.setMove(1, 2, 3, 4);
    EXPECT_EQ(moveCommand.getMove(), "1 2 3 4");
}


