/* 
This is a test sketch for the Catoptric assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control
For use with the Catoptric Motor Shield v2 
---->  http://www.Catoptric.com/products/1438
*/


#include <Wire.h>
#include <Catoptric_MotorShield.h>
#include "utility/Catoptric_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
//Catoptric_MotorShield AFMS = Catoptric_MotorShield(0x43); 
// Or, create it with a different I2C address (say for stacking)
//Catoptric_MotorShield AFMS = Catoptric_MotorShield(0x63); 
#define NUM_ROWS 3
Catoptric_MotorShield AFMS[NUM_ROWS];
Catoptric_StepperMotor *motors[NUM_ROWS][2];

Catoptric_StepperMotor *motor;

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
//Catoptric_StepperMotor *myMotor = AFMS.getStepper(200, 1);

boolean done = false;

void setup() {
  // Open serial connection
  Serial.begin(9600);
  
  // Initialize shields and motors
  for (int i = 0; i < NUM_ROWS; i++) {
    AFMS[i] = Catoptric_MotorShield(i + 0x40);
    //Serial.println("here");
    for (int j = 0; j < 2; j++) {
      motors[i][j] = AFMS[i].getStepper(200, (j+1));
    }
    Serial.print("Initializing ");
    Serial.println(i+1);
    AFMS[i].begin();
    Serial.println("Are you done?");
  }
}

void loop() {
  if (done) return;
  for (int i = 0; i < NUM_ROWS; i++) {
    Serial.print("Mirror ");
    Serial.println(i+1);
    motor = motors[i][0];
    motor->setSpeed(10);
    motor->step(100, BACKWARD, SINGLE);

    motor = motors[i][1];
    motor->setSpeed(10);
    motor->step(100, BACKWARD, SINGLE);
    motor->step(100, FORWARD, SINGLE);
    motor->release();

    motor = motors[i][0];
    motor->step(100, FORWARD, SINGLE);  
    motor->release();
    }
  
  done = true;
  Serial.println("Done");
}
