void SetupJSN() {
  //Serial.begin(115200);
  pinMode(JSNPIN2, INPUT);
  pinMode(JSNPIN1, OUTPUT);
}

void JSNCheck() {
  digitalWrite(JSNPIN1, LOW);
  delayMicroseconds(2);
  digitalWrite(JSNPIN1, HIGH);
  delayMicroseconds(20);
  digitalWrite(JSNPIN1, LOW);

  float duration = pulseIn(JSNPIN2, HIGH);
  float distance = (duration / 2) * 0.0343;
  JSN = 100 * distance / JSNHEIGHT;
  if (JSN > 100) {
    JSN = 100;
  } else if (JSN < 0) {
    JSN = 0;
  }
  JSN = 100 - JSN;
  /*if(digitalReadOutputPin(JSNMINCONTROL)){

  }*/
  if(!dROP(JSNMINCONTROL) && JSN < JSNMIN){//Si el nivel del agua sobrebaja el mínimo y está apagado, se enciende el control.
    digitalWrite(JSNMINCONTROL, HIGH);
  }else
  if(dROP(JSNMINCONTROL) && JSN >= JSNMAX){//Si el nivel del agua supera el máximo y está encendido, se apaga el control.
    digitalWrite(JSNMINCONTROL, LOW);
  }
  if (alternadorLCD == N_JSN) {
    lcd.setCursor(0, 0);
    lcd.print("Porcentaje: " + String(JSN) + "%");
    lcd.setCursor(0, 1);
    lcd.print("                    ");
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print("                  ");
  }
}
void JSNLocalSave(String date) {
  if (SaveSensorValue("JSN", date, (JSN == 0 ? "NULL" : String(JSN)))) {
    NoSD();
  }
}

void JSNUpToUbi(String DATE) {
  if (lux != -2) {
    if (ubidots.connected()) {
      ubidots.add("nivel", JSN);
      ubidots.publish(DEVICE_LABEL.c_str());
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('nivel', '" + DATE + "','" + JSN + "', 'no')").c_str());
    }
  }
}