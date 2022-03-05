// Manual control breaks, speeds, disable and enable driver or other functions
void manual(int switcher, String channel, String amps) {
  switch (switcher) {
    case 8000: // enable driver, apply pwm
      digitalWrite(enablePin, HIGH);
      ampPWM = amps.toFloat();
      motorControl(ampPWM, ledChannel[channel.toInt()]);
      break;
    case 8001: // enable driver, apply pwm
      digitalWrite(enablePin, HIGH);
      ampPWM = amps.toFloat();
      motorControl(ampPWM, ledChannel[0]);
      motorControl(ampPWM, ledChannel[1]);
      motorControl(ampPWM, ledChannel[2]);
      break;
    case 9000: // enable driver
      digitalWrite(enablePin, HIGH);
      break;
    case 9001: // disable driver set pwm to lowPWM
      digitalWrite(enablePin, LOW);
      ledcWrite(ledChannel[0], nullPWM);
      ledcWrite(ledChannel[1], nullPWM);
      ledcWrite(ledChannel[2], nullPWM);
      break;
  }
}
