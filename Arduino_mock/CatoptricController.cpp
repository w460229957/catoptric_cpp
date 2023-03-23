#include "Serial.hpp"
#include <string>
#include <iostream>
#include <unordered_map>
#include <sys/epoll.h>

class Catoptric_StepperMotor{
public:
    Catoptric_StepperMotor(int steps, int motorNum);
    ~Catoptric_StepperMotor();
    void setSpeed(long whatSpeed);
    void step(long steps, int dir, int style);
    void release();
};


class Catoptric_MotorShield{
private:
    std::unordered_map<int, Catoptric_StepperMotor*> motors;
public:
    Catoptric_MotorShield();
    Catoptric_MotorShield(int addr);
    ~Catoptric_MotorShield();
    Catoptric_StepperMotor *getStepper(int steps, int motorNum);
};


// Create the motor shield object with the default I2C address
//Catoptric_MotorShield CFMS = Catoptric_MotorShield(0x43); 
// Or, create it with a different I2C address (say for stacking)
//Catoptric_MotorShield CFMS = Catoptric_MotorShield(0x63); 
#define NUM_ROWS 3
Catoptric_MotorShield CFMS[NUM_ROWS];
Catoptric_StepperMotor *motors[NUM_ROWS][2];

Catoptric_StepperMotor *motor;

// Connect a stepper motor with 100 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
//Catoptric_StepperMotor *myMotor = CFMS.getStepper(100, 1);

//pin 13
char done = 0;

#define MAGIC_NUM 33
#define NUM_ROWS 3

#define GET_MAGIC_NUM 0
#define GET_KEY 1
#define GET_X 2
#define GET_Y 3
#define GET_M 4
#define GET_D 5
#define GET_COUNT_HIGH 6
#define GET_COUNT_LOW 7
#define SINGLE 1

int currentState = GET_MAGIC_NUM;
int x;
int y;
int m;
int d;
int countHigh;
int countLow;
int led = 3; // the pin the LED is connected to

void nack(std::string);
void stepMotor(int, int, int, int, int);
void ackA(int, int, int);
void msgInt(int);
void msgStr(std::string);

Serial Serial{};

int main(){
}

void nack(std::string message) { 
  std::string prefix = "Nack: ";
  message = prefix + message;
  
  int len = message.length();
  //int iLow;
  //int iHigh;

  //iLow = (len >> 8) && 255;
  //iHigh = len && 255;
  /* 
    message format: |MAGIC_NUM|b|message payload|#|
                      4 bytes  1  len(message)   1 byte
  */
  Serial.write(MAGIC_NUM);
  Serial.write('b');
  //Serial.write(iHigh);
  //Serial.write(iLow);
  for (int i = 0; i < len; i++) {
    Serial.write(message[i]);
  }
  Serial.write('#');
}

void stepMotor(int x, int y, int m, int d, int count) {
  int dir = d+1;
  motors[y][m]->setSpeed(10); 
  motors[y][m]->step(count, dir, SINGLE);
  ackA(x, y, m);
  motors[y][m]->release(); 
}

void ackA(int x, int y, int m) {
  //msgStr("Sending Acknowledge Message");
  Serial.write(MAGIC_NUM);
  Serial.write('a');
  //Serial.write('A');
  Serial.write(x);
  Serial.write(y);
  Serial.write(m);
  Serial.write('#');
}



void setup() {
  
  //Serial.println("Before loop setup");
  // Initialize shields and motors
  for (int i = 0; i < NUM_ROWS; i++) {
    CFMS[i] = Catoptric_MotorShield(i + 0x40);
    for (int j = 0; j < 2; j++) {
      motors[i][j] = CFMS[i].getStepper(100, (j+1));
    }
    //Serial.print("Initializing ");
    //Serial.println(i+1);
    CFMS[i].begin();
  }
}



void loop() { 
  while (Serial.available() >= 1){ 
    //Serial.write(m);
    unsigned char c = Serial.read();
    int nextState = currentState; //Default nextState
    msgStr("Testing message from Aduino");
    msgInt(nextState);
    switch (currentState) {

      case GET_MAGIC_NUM:
        if (c == MAGIC_NUM){
          nextState = GET_KEY;
        }
        break;

      case GET_KEY:
        //key = c;
        switch (c) {
          case 65:
            nextState = GET_X;
            //msgStr("Key");
            break;
          default:
            nack("Invalid Key");
            nextState = GET_MAGIC_NUM;
            break;
        }
        break;

      case GET_X: //row
        if (c <= 2) {
          x = c;
          //msgStr("X");
          nextState = GET_Y;
        }
        else {
          nack("Invalid X");
          nextState = GET_MAGIC_NUM;
        }
        break;

      case GET_Y: //columns
        //msgStr("Y");
        if (c <= NUM_ROWS) {
          y = c;
          nextState = GET_M;
        }
        else {
          nack("Invalid Y");
          nextState = GET_MAGIC_NUM;
        }
        break;

      case GET_M:
        if (c <= 2) {
          m = c;
          //msgStr("M");
          nextState = GET_D;
        }
        else {
          nack("Invalid M");
          nextState = GET_MAGIC_NUM;
        }
        break;

      case GET_D:
        if (c <= 1) {
          d = c;
          //msgStr("count");
          nextState = GET_COUNT_HIGH;
        }
        else {
          nack("Invalid D");
          nextState = GET_MAGIC_NUM;
        }
        break;

      case GET_COUNT_HIGH:
        countHigh = c;
        nextState = GET_COUNT_LOW;
        break;

      case GET_COUNT_LOW:
      
        //countLow = c;
        int count = (countHigh << 8) + c; 

        digitalWrite(led, HIGH);
        stepMotor(x, y, m, d, count);
        digitalWrite(led, LOW);
    
        
        nextState = GET_MAGIC_NUM;
        break;

    }
    currentState = nextState;
  }
}