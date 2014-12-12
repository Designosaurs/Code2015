void goalGrabberUp() {
	servo[goal_grab] = 60;
}
void goalGrabberDown() {
	servo[goal_grab] = 23;
}

void placerDown() {
	servo[placer] = 255;
}

void placerPlace() {
	servo[placer] = 85;
}

const int INIT_ELBOW_POS = 128;
void initElbow() {
	servo[elbow] = INIT_ELBOW_POS;
	elbowPos = INIT_ELBOW_POS;
}

const int INIT_WRIST_POS = 128;
void initWrist() {
	servo[wrist] = INIT_WRIST_POS;
	wristPos = INIT_ELBOW_POS;
}