
#include <sys/ioctl.h>
#include <cstring>
#include <termios.h>
#include <vector>
#include <unistd.h>

#include "CatoptricRow.hpp"
#include "SerialFSM.hpp"
#include "prep_serial.hpp"
#include "ErrCodes.hpp"

using namespace std;

MotorState::MotorState() {
    motor[PAN_IND] = 0;
    motor[TILT_IND] = 0;
}

MotorState::MotorState(int panIn, int tiltIn) {
    motor[PAN_IND] = panIn;
    motor[TILT_IND] = tiltIn;
}

Message::Message(int row_in, int mirror_in, int motor_in, int dir_in, 
        int chigh_in, int clow_in) {
    rowNum = row_in;
    mirrorID = mirror_in;
    whichMotor = motor_in;
    direction = dir_in;
    countHigh = chigh_in;
    countLow = clow_in;
}

Message::Message(int mirrorRow, int mirrorColumn, int motorNumber, 
        int position) {
    rowNum = mirrorRow;
    mirrorID = mirrorColumn;
    whichMotor = motorNumber;
    newPos = position;
}

/* Convert the message into a serialized byte vector that can be sent to
 * the Arduino.
 */
vector<char> Message::toVec() {

    vector<char> msgVec;
    msgVec.push_back(MSG_MAGIC_NUM);
    msgVec.push_back(ACK_KEY);
    msgVec.push_back(rowNum);
    msgVec.push_back(mirrorID);
    msgVec.push_back(whichMotor);
    msgVec.push_back(direction);
    msgVec.push_back(countHigh);
    msgVec.push_back(countLow);

    return msgVec;
}

/* Convert the Message object into a string containing what will be transmitted.
 */
string Message::toStr() {

    string str;
    try {
        str = to_string(MSG_MAGIC_NUM) + to_string(ACK_KEY) + 
            to_string(rowNum) + to_string(mirrorID) + to_string(whichMotor) +
            to_string(direction) + to_string(countHigh) + to_string(countLow);
    } catch (...) {
        printf("to_string error: %s\n", strerror(errno));
        return string();
    }

    return str;
}

CatoptricRow::CatoptricRow() {}

CatoptricRow::CatoptricRow(int rowNumberIn, int numMirrorsIn, 
        const char *serialPortIn) {

	rowNumber = rowNumberIn;
	numMirrors = numMirrorsIn;

	// Initalize a MotorState object for each motor
	for(int i = 0; i < numMirrors; ++i) {
        MotorState state = MotorState();
	    motorStates.push_back(state);
    }

	// Configure the relevant serial port for communication
	setupSerial(serialPortIn);

    // Create FSM for communication with this Arduino.
    string rowStr = to_string(rowNumber);
    const char *rowCStr = rowStr.c_str();
	fsm = SerialFSM(rowCStr);
}

/* Prepare the correpsonding serial port for IO (termios).
 */
int CatoptricRow::setupSerial(const char *serialPortIn) {
    
    // Returns fd for configured serial port
    serial_fd = prep_serial(serialPortIn); 
    
    // Flush residual data in buffer
    if(tcflush(serial_fd, TCIOFLUSH) < 0) {
        printf("tcflush error: %s\n", strerror(errno));
        return ERR_TCFLUSH;
    }
    
    sleep(SETUP_SLEEP_TIME); // Why does this function sleep?

    return RET_SUCCESS;
}

/* Flush the serial port buffer.
 */
int CatoptricRow::resetSerialBuffer() {
    if(tcflush(serial_fd, TCIOFLUSH) < 0) {
        printf("tcflush error: %s\n", strerror(errno));
        return ERR_TCFLUSH;
    }

    return RET_SUCCESS;
}

/* Read all input from the corresponding serial port and update the CatoptricRow
 * object's SerialFSM object.
 * Send a Message object from the back of the commandQueue to the Arduino.
 */
void CatoptricRow::update() {
    
    char input;
    while(read(serial_fd, &input, 1) > 0) {
        fsm.Execute(input);
        if(fsm.messageReady) {
            printf("Incoming message:%s\n", fsm.message);
            fsm.clearMsg();
        }
    }

	while(fsmCommandsOut() < MAX_CMDS_OUT && commandQueue.size() > 0) { 
		Message message = commandQueue.back();
        commandQueue.pop_back();
		sendMessageToArduino(message);
	    fsm.currentCommandsToArduino--;
    }
}

/* Transmit the passed Message to the Arduino.
 */
void CatoptricRow::sendMessageToArduino(Message message) {

    vector<char> message_vec = message.toVec();
    char bCurrent;

	for(int i = 0; i < NUM_MSG_ELEMS; ++i) {
		bCurrent = message_vec[i];
        int writeRet = write(serial_fd, &bCurrent, 1);  // TODO : undo this
        //printf("\twriteRet %d\n", writeRet);
        if(writeRet < 0) {
            printf("write error: %s\n", strerror(errno));
            return;
        }
    }

    printf("Successfully sent message to Arduino:"); 
    vector<char> msgVec = message.toVec();
    for(char c : msgVec) printf("%3d ", (unsigned) c);
    printf("\n");

	fsm.currentCommandsToArduino += 1; // New sent message, awaiting ack
}

/* Push a Message onto the commandQueue to update a mirror's position.
 */
void CatoptricRow::stepMotor(int mirrorID, int whichMotor, 
        int direction, float deltaPos) {

    // I assume there's 513 steps in the motor?
	int deltaPosInt = (int) (deltaPos * (513.0/360.0));
	int countLow = ((int) deltaPosInt) & 255;
	int countHigh = (((int) deltaPosInt) >> 8) & 255;
    
    // mirrorID could just as well be named columnNumber
	Message message (rowNumber, mirrorID, whichMotor, direction, 
            countHigh, countLow);
	commandQueue.push_back(message);
}

/* Push a Message onto the commandQueue and update motorStates. 
 * TODO : There may be functional redundancy between stepMotor() and 
 *        reorientMirrorAxis()?
 */
void CatoptricRow::reorientMirrorAxis(Message command) {

    int mirror = command.mirrorID;
    int motor = command.whichMotor;
    int newState = command.newPos;
    int currentState = -1; // Placeholder value to silence compiler warnings :/

    if(motor == PAN_IND) {
        currentState = motorStates[mirror - 1].motor[PAN_IND];
    } else if(motor == TILT_IND) {
        currentState = motorStates[mirror - 1].motor[TILT_IND];
    } else {
        printf("Invalid 'motor' value in reorientMirrorAxis\n");
        return;
    }

    int delta = newState - currentState;
    int direction = delta <  0 ? 1 : 0;
    if(delta < 0) delta *= -1;

    stepMotor(mirror, motor, direction, delta);
    motorStates[mirror - 1].motor[motor] = newState;
}

/* Reset the orientation of all mirrors in this row or test functionality
 * of the motors/connection,, depending on value of 'test' parameter.
 */
void CatoptricRow::reset(bool test) {
    // Column numbers seem to not be 0-indexed on the Arduino?

    if(test) {
        for(int i = 0; i < numMirrors; ++i) {
            /* Orientation of mirrors isn't guaranteed after executing this, 
             * shouldn't use with 'test'=true for anything but debugging! */
            stepMotor(i + 1, PAN_IND, MOTOR_FORWARD, MOTOR_TEST_ARC);
            stepMotor(i + 1, PAN_IND, MOTOR_BACKWARD, MOTOR_TEST_ARC);
            stepMotor(i + 1, TILT_IND, MOTOR_FORWARD, MOTOR_TEST_ARC);
            stepMotor(i + 1, TILT_IND, MOTOR_BACKWARD, MOTOR_TEST_ARC);
        }
    } else {
        for(int i = 0; i < numMirrors; ++i) {
            stepMotor(i + 1, PAN_IND, MOTOR_BACKWARD, MOTOR_RESET_ARC);
            stepMotor(i + 1, TILT_IND, MOTOR_BACKWARD, MOTOR_RESET_ARC);
        }
    }

    update();
}

/* Get the SerialFSM's currentCommandsToArduino */
int CatoptricRow::fsmCommandsOut() {
	return fsm.currentCommandsToArduino;
}

/* Get the SerialFSM's nackCount */
int CatoptricRow::fsmNackCount() {
	return fsm.nackCount;
}

/* Get the SerialFSM's ackCount */
int CatoptricRow::fsmAckCount() {
	return fsm.ackCount;
}

/* Get the CatoptricRow's rowNumber */
int CatoptricRow::getRowNumber() {
    return rowNumber;
}

void CatoptricRow::cleanup() {
    close(serial_fd);
    fsm.clearMsg();
}
