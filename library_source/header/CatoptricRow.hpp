#pragma once

#include <memory>
#include <vector>
#include "SerialFSM.hpp"
#include <mutex>
#include "CommandQueue.hpp"
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
    Message(int mirrorRow, int mirrorColumn, int motorNumber, int direction, 
        int position, int countHigh, int countLow)
        :
        rowNum(mirrorRow), mirrorID(mirrorColumn), whichMotor(motorNumber),
        direction(direction), newPos(position), countHigh(countHigh),
        countLow(countLow) {};
    Message(int mirrorRow, int mirrorColumn, int motorNumber, int direction, 
        int countHigh, int countLow)
        :
        rowNum(mirrorRow), mirrorID(mirrorColumn), whichMotor(motorNumber),
        direction(direction), newPos(0), countHigh(countHigh),
        countLow(countLow) {};
    Message(int mirrorRow, int mirrorColumn, int motorNumber, int direction, 
    int position)
    :Message(mirrorRow, mirrorColumn, motorNumber, direction,
    position, 0, 0) {};
    Message(int mirrorRow, int mirrorColumn, int motorNumber, int position):
        Message(mirrorRow, mirrorColumn, motorNumber, 0, position) {};

    std::vector<char> toVec(); 
    //define a conversion operator to convert a Message to a string
    operator std::string() const;
    friend std::ostream& operator<<(std::ostream& os, const Message& msg);
};
/* Encodes the state of a row of mirrors/motors and the corresponding Arduino */
class CatoptricRow {

    private:
        //Number of messages in the command queue

        // File descriptor of serial port written to for its Arduino
        int serial_fd;
        int rowNumber, numMirrors;
        /* Vector of the current (theoretical) orientations for
           each subordinate motor */
        std::vector<MotorState> motorStates;

        int setupSerial(const std::string&);
        void sendMessageToArduino(Message);


    public:
        void stepMotor(int, int, int, 
                float); 
        // Queue of pending Message objects to be transmitted to Arduino
        std::shared_ptr<CommandQueue<Message>> commandQueue;
        // FSM controlling this Arduino
        SerialFSM fsm;
        CatoptricRow(int & , int &,std::string&);
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
