#pragma once

#include <vector>
#include "SerialFSM.hpp"

#define MAX_CMDS_OUT  32
#define FLUSH_IN_OUT  2
#define NUM_MSG_ELEMS 8
#define NUM_MOTORS    2
#define SETUP_SLEEP_TIME 2
#define MOTOR_RESET_ARC 200
#define MOTOR_TEST_ARC 80

#define MSG_MAGIC_NUM '!'
#define ACK_KEY 'A'

// Indices for values storing orientations of each motor in each mirror unit
#define PAN_IND  0
#define TILT_IND 1

// TODO : Not sure these values are correct, may need to swap them!
#define MOTOR_FORWARD  1
#define MOTOR_BACKWARD 0

/* Encodes the state of mirror motors unit */
struct MotorState {
    int motor[2];

    MotorState();
    MotorState(int panIn, int tiltIn);
};

/* Encodes a message to be sent to an Arduino */
struct Message {

    int rowNum, mirrorID, whichMotor, direction, newPos;
    int countHigh, countLow;
    Message(int row_in, int mirror_in, int motor_in, int dir_in, 
            int chigh_in, int clow_in);
    Message(int mirrorRow, int mirrorColumn, int motorNumber, int position);
    std::vector<char> toVec(); 
};

/* Encodes the state of a row of mirrors/motors and the corresponding Arduino */
class CatoptricRow {

    private:
        // File descriptor of serial port written to for its Arduino
        int serial_fd;
        int rowNumber, numMirrors;
        /* Vector of the current (theoretical) orientations for
           each subordinate motor */
        std::vector<MotorState> motorStates;

        int setupSerial(const char *serial_port_in);
        void sendMessageToArduino(Message message);


    public:
        void stepMotor(int mirrorID, int whichMotor, int direction, 
                float delta_pos); 
        // Queue of pending Message objects to be transmitted to Arduino
        std::vector<Message> commandQueue;
        // FSM controlling this Arduino
        SerialFSM fsm;

        CatoptricRow();
        CatoptricRow(int rowNumber_in, int numMirrors_in, 
                const char *serial_port_in);

	    void reset(bool test);
        void update();
        int resetSerialBuffer();
        int getRowNumber();
        int fsmCommandsOut(); 
        int fsmNackCount();
        int fsmAckCount();
	    void reorientMirrorAxis(const Message & command); 
        void cleanup();
};
