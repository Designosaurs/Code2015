void goForwardTime(float seconds, int power) {
    motor[left_drive] = power;
    motor[right_drive] = power;
    avg_speed = power;
    wait1Msec(seconds * 1000);
}

void goForwardDistance(float feet, int power) {

}

void stop() {
    motor[left_drive] = 0;
    motor[right_drive] = 0;
}

void goBackwardTime(float seconds, int power) {
    motor[left_drive] = -power;
    motor[right_drive] = -power;
    avg_speed = -power;
    wait1Msec(seconds * 1000);
}

void goBackwardDistance(float feet, int power) {

}

void accel(int to_speed) {
    to_speed = (to_speed * 100) / MAX_SPEED;
    for (int i = 0; i < to_speed; i = i + 0.2 * to_speed) {
        goForwardTime(0.1, i);
    }
}

void decel(int to_speed) {
    to_speed = (to_speed * 100) / MAX_SPEED;
    for (int i = avg_speed; i < to_speed; i = i - 0.2 * to_speed) {
        goForwardTime(0.1, i);
    }
}