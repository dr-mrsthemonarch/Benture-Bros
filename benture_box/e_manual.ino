// Manual control breaks, speeds, disable and enable driver or other functions
float manual(int manualControl, String v1, String v2, String v3) {
  switch (manualControl) {

    case 7001: // Omega offset
      offsetOmega = v3.toFloat();
      break;

    case 7002: // Angle offset
      offsetAngle = v3.toFloat();
      //alpha = v3.toFloat();
      break;

    case 8000: // enable driver, apply pwm
      digitalWrite(enablePin, HIGH);
      ampPWM = v3.toFloat();
      manualAmps = ampsToPWM(ampPWM); // used in manual control
      motor(ledChannel[0], manualAmps);
      motor(ledChannel[1], manualAmps);
      motor(ledChannel[2], manualAmps);
      break;

    case 9000: // disable driver set pwm to lowPWM
      digitalWrite(enablePin, LOW);
      ledcWrite(ledChannel[0], nullPWM);
      ledcWrite(ledChannel[1], nullPWM);
      ledcWrite(ledChannel[2], nullPWM);
      break;

    case 9001: // enable driver, speed a direction, set pwm to lowPWM
      digitalWrite(enablePin, HIGH);
      break;
  }
}
