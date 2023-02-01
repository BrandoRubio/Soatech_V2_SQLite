int buf[10], temp11;

void PHCheck() {
  PH = getPH();
  //control para subir PH
  if (PHMINCONTROL && !dROP(PHMINCONTROL) && PH <= PHMIN) {
    DataLogger("Control para subir PH", 0);
    digitalWrite(PHMINCONTROL, VHIGH);
  } else if (PHMINCONTROL && dROP(PHMINCONTROL) && PH >= (PHIDEAL - 1)) {
    DataLogger("Apaga control para subir PH", 0);
    digitalWrite(PHMINCONTROL, VLOW);
  }
  //control para bajar PH
  if (PHMAXCONTROL && !dROP(PHMAXCONTROL) && PH >= PHMAX) {
    digitalWrite(PHMAXCONTROL, VHIGH);
    DataLogger("Control para bajar PH", 0);
  } else if (PHMAXCONTROL && dROP(PHMAXCONTROL) && PH <= (PHIDEAL + 1)) {
    DataLogger("apaga control para bajar PH", 0);
    digitalWrite(PHMAXCONTROL, VLOW);
  }
  if (alternadorLCD == N_PH) {
    lcd.setCursor(0, 0);
    lcd.print("PH:" + String(PH) + "           ");
    lcd.setCursor(0, 1);
    lcd.print("                    ");
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
  }
}
void PHLocalSave(String date) {
  if (SaveSensorValue("ph", date, (PH == -127 ? "NULL" : String(PH)))) {
    NoSD();
  }
}
void PHUpToUbi(String DATE) {
  if (ubidots.connected()) {
    ubidots.add("ph", PH);
    ubidots.publish(DEVICE_LABEL.c_str());
  } else {
    db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('ph', '" + DATE + "','" + PH + "', 'no')").c_str());
  }
}
float getPH() {
  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(PHPIN);
    delay(30);
  }
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp11 = buf[i];
        buf[i] = buf[j];
        buf[j] = temp11;
      }
    }
  }
  long int avgValue = 0;
  for (int i = 2; i < 8; i++)
    avgValue += buf[i];
  float pHVol = ((((float)avgValue) * 5.0) / 4095.0) / 6;
  float PH = (-2.9718 * pHVol) + 18.6;
  return (PH);
}
