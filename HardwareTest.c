#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,          Red,           tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          Yellow,        tmotorNXT, openLoop)
#pragma config(Motor,  motorC,          Green,         tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     Shoulder,      tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C2_2,     Elbow,         tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C3_1,     RightDrive,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     LeftDrive,     tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    LClaw,                tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    RClaw,               tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    Claw,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
/*
|----\             
|    | /--\ /--- - /--\ /--\ /--\ /--- /--\ |  | |-- /---
|    | |--/ \--\ | \--| |  | |  | \--\ |  | |  | |   \--\
|----/ \__  ---/ | __/  |  | \--/ ---/ \--\ \--\ |   ---/  Team 6369
*/


// About the setup:
// Shoulder and Elbow are both set up so postive motor => increasing encoder count.


#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages coming from the controller.
#include "buttons.h"

#define TEST_POWER 10

void Drive( void )
{
	int y1;
	int y2;

	y1 = 	 joystick.joy1_y1;
	y2 = 	 joystick.joy1_y2;

	if (abs(y1) > 10)
	{
		motor[LeftDrive] = -y1 / 3;
	}
	else
	{
		motor[LeftDrive] = 0;
	}

	if (abs(y2) > 10)
	{
		motor[RightDrive] = y2 / 3;
	}
	else
	{
		motor[RightDrive] = 0;
	}
}

void TestArmMotors( void )
{
	switch( joystick.joy1_TopHat)
	{
	case 0:
		motor[Shoulder]= TEST_POWER;
		motor[Elbow]= 0;
		break;
	case 4:
		motor[Shoulder]= -TEST_POWER;
		motor[Elbow]= 0;
		break;

	case 2:
		motor[Elbow] = TEST_POWER;
		motor[Shoulder]= 0;
		break;

	case 6:
		motor[Elbow] = -TEST_POWER;
		motor[Shoulder]= 0;
		break;


	default:
		motor[Elbow]= 0;
		motor[Shoulder]=0;
	}

}


void TestServos( void )
{
	if (joystick.joy1_Buttons == button4)
	{
		servo[RClaw] = 100;
	}

	if (joystick.joy1_Buttons == button2)
	{
		servo[RClaw] = 128;
	}

	if (joystick.joy1_Buttons == button5)
	{
		servo[Claw] = 0;
	}

	if (joystick.joy1_Buttons == button7)
	{
		servo[Claw] = 64;
	}


	if (joystick.joy1_Buttons == button1)
	{
		servo[LClaw] = 0;
	}

	if (joystick.joy1_Buttons == button3)
	{
		servo[LClaw] = 64;
	}


}

void TestEncoders( void )
{
	int ShoulderEnc;
	int ElbowEnc;

	ShoulderEnc = nMotorEncoder[Shoulder];
	ElbowEnc = nMotorEncoder[Elbow];

	nxtDisplayString(3, "Shd: %6d", ShoulderEnc);
	nxtDisplayString(4, "Elb: %6d", ElbowEnc);
	nxtDisplayTextLine(5, "Ext Batt:%4.1f V", externalBatteryAvg / (float) 1000);

}


task main()
{
	bDisplayDiagnostics = false;  // Disables the diagnostic display in JoyStickDriver.c

	//servoChangeRate[RClaw] = 3;
	//servoChangeRate[LClaw] = 3;
	//servoChangeRate[Claw] = 3;

	servo[RClaw] = 128;
	while(1)
	{
		getJoystickSettings(joystick);     // update buttons and joysticks
		Drive();
		TestArmMotors();
		TestServos();
		TestEncoders();
	}



}
