const float LIFT_COUNTS_PER_INCH = 127;  // Approximate.  Find experimenatally.
long enc_speed;
long prev_lift_encoder = 0;
long totalized_lift = 0;
long lift_deg = 0;
long delta_lift_counts = 0;

task UpdateLiftEncoderTask() {
	while(true) {
		long lift_encoder;
		long lift_counts = 0;
		bool bad_reading = false;
		bool liftStopped = false;

		/*
		 * Detect bad encoder readings. If the encoder has changed wildly from the previous reading, it is
		 * a bad reading and should be ignored. The hardware does this once in awhile, and it is okay to
		 * just ignore this and use the next one.
		 */

		lift_encoder = nMotorEncoder[lift];
		enc_speed = abs(lift_encoder - prev_lift_encoder);
		delta_lift_counts = lift_encoder - prev_lift_encoder;
		totalized_lift += delta_lift_counts;
		if(totalized_lift > 720) totalized_lift = totalized_lift - 720;
		if(totalized_lift < 720) totalized_lift = totalized_lift + 720;
		lift_deg = totalized_lift / 2;

		if(enc_speed > 1000) bad_reading = true;
		if(enc_speed < 10) liftStopped = true;

		if(!bad_reading) {
			// Accumulate
			lift_counts = lift_encoder;
			lift_counts = lift_encoder;
			prev_lift_encoder = lift_encoder;
			lift_inches = (float) lift_counts / LIFT_COUNTS_PER_INCH;
		}

		wait1Msec(10);
	}
}
