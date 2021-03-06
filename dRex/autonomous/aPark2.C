#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ultrasonic,     sensorSONAR)
#pragma config(Sensor, S3,     HTIRS2,              sensorI2CCustom)
#pragma config(Sensor, S4,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorA,           motorA,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     right_drive,   tmotorTetrix, PIDControl, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     left_drive,    tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C3_1,     lift,          tmotorTetrix, PIDControl, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     harvester,     tmotorTetrix, PIDControl)
#pragma config(Servo,  srvo_S1_C2_1,    placer,               tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    goal_grab,            tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    elbow,                tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    wrist,                tServoStandard)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
_____             _
|  __ \          (_)
| |  | | ___  ___ _  __ _ _ __   ___  ___  __ _ _   _ _ __ ___
| |  | |/ _ \/ __| |/ _` | '_ \ / _ \/ __|/ _` | | | | '__/ __|
| |__| |  __/\__ \ | (_| | | | | (_) \__ \ (_| | |_| | |  \__ \
|_____/ \___||___/_|\__, |_| |_|\___/|___/\__,_|\__,_|_|  |___/
__/ |
|___/                            Team #6369
Assumes IR beacon points straight ahead.  (prior to now it did not!)
*/

#include "JoystickDriver.c"
#include "a_globals.h"
#include "..\common\bot_specific.h"
#include "..\common\init.h"
#include "..\common\functions.h"
#include "..\common\servo.h"
#include "..\sensor\drivers\hitechnic-irseeker-v2.h"
#include "..\sensor\GetIR.h"
#include "..\sensor\ultrasonic.h"
#include "..\common\macros.h"
#include "..\common\UpdateLiftEncoderTask.h"
#include "..\common\UpdateDisplayTask.h"
#include "..\common\UpdateDriveBearings.h"
//#include "..\common\HarvesterTask.h"
#include "drive.h"
#include "sonar.h"


// Usage: start from the center of the parking zone.
// Go for a fixed distance.  Then  you know if the range is:
// Low - the skinnny end is pointed toward you.  Typical dist is 51
// Mid - the flat end is pointed toward you.  Typ dist is 68
// Hi (no reading => 255) the goal is at 45.
int getGoalState() {
	int range = getClosestRange();
	writeDebugStreamLine("%d", range);
	if(range < 65) {
		// |
		return 1;
		} else if(range < 100) {  // Look for flat goal
			// Be sure that it is not clutter from the angled goal.
			pivotDegrees( 10, 50);
			range = getClosestRange();
			if (range > 120 ) {
				pivotDegrees( 15, 60);
				return 3;
			}
			pivotDegrees( -15, 60);
			return 2;
		} else {
		// /
		return 3;
	}
}

void knockKickstand() {
	pivotDegrees(70, 60);
	goForwardDistance(1.5, 60);
	pivotDegrees(-75, 60);
	goForwardDistance(2, 100);
	goForwardDistance(.5, 65);
}


void placeInCenter() {
	// Point toward the IR beacon, even though we are a ways from it.
	if (PointToIR() == false) stopAndWait();
	// Now get the ideal distance for placing:
	if (FollowIRtoRange( 47 ) == false) stopAndWait();
	//goToRange(49, 30);
	// Record the angle before the last IR adjustment, as it is likely lined up well with
	// the center goal structure as a whole, if not the center goal:
	angle_before_ir = total_angle;
	if (PointToIR() == false) stopAndWait();
	// Compensate here if the cup is not exactly over the IR beacon:
	pivotDegrees( -4, 30);
	stop();
	TossToCenterGoal();
	//stopAndWait();
	liftToFloor();
	pivotToTotalAngle(angle_before_ir, 40);
	stop();
	//stopAndWait();
}

task main() {
	initDisplay();
	goalGrabberUp();
	initPlacer();
	initDriveConfig();

	// 	Initial position for cup is tilted a little up, so we do not lose
	// The small balls. Smaller number = pointed more up
	servo[wrist] = 80;
	wristPos = (float) 80;
	servo[elbow] = 217;
	elbowPos = 217;

	waitForStart();

	eraseDisplay();
	StartTask(UpdateLiftEncoderTask);
	//StartTask(HarvesterTask);  If you start this total_angle goes wrong.
	StartTask(UpdateDriveBearingsTask);
	StartTask(UpdateDisplayTask);

	// Test toss code
	//TossToCenterGoal();
	//wait1Msec(10000);
	//tuck();
	//stopAndWait();

	goForwardDistance(2, 40);
	stop();
	//stopAndWait();
	switch(getGoalState()) {
	case 1:  // Skinny side facing us
		placeInCenter();
		break;
	case 2: //---  Flat side facing us
		PlaySound(soundFastUpwardTones);
		pivotDegrees(-60, 60);
		goForwardDistance(4, 65);
		//pivotToTotalAngle(0, 50);
		//goForwardDistance(0.2, 40);
		pivotToTotalAngle(95, 60);
		stop();
		//stopAndWait();
		placeInCenter();
		break;
	case 3:  /// / Angled goal
		PlaySound(soundException);
		// These moves should get you facing the center goal, but aways from it:
		pivotDegrees(-85, 60);
		goForwardDistance(2.6, 70);
		pivotToTotalAngle(45, 80);
		placeInCenter();
		break;
	}
	knockKickstand();
	stopAndWait();
}
