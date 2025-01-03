#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

unsigned short MotorSetup();
unsigned short MotorControlSetRightMotorSpeed(short speed);
unsigned short MotorControlSetLeftMotorSpeed(short speed);
unsigned short MotorControlStopRightMotor();
unsigned short MotorControlStopLeftMotor();

#endif // MOTOR_CONTROL_H
