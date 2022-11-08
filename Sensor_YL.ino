void YLCheck() {
  float sh1 = map(analogRead(YLPIN1), 0, 4095, 100, 0);
  float sh2 = map(analogRead(YLPIN2), 0, 4095, 100, 0);
  float sh3 = map(analogRead(YLPIN3), 0, 4095, 100, 0);
  float sh4 = map(analogRead(YLPIN4), 0, 4095, 100, 0);
  S_HUM = (sh1 + sh2 + sh3 + sh4) / 5;

  if (alternadorLCD == NS_H) {
    lcd.setCursor(0, 0);
    lcd.print("H1:" + String(sh1) +" ");
    lcd.setCursor(9, 0);
    lcd.print(" H2:" + String(sh2));
    lcd.setCursor(0, 1);
    lcd.print("H3:" + String(sh3) +" ");
    lcd.setCursor(9, 1);
    lcd.print(" H4:" + String(sh4));
    lcd.setCursor(0, 2);
    lcd.print("Promedio: " + String(S_HUM));
  }
}
void YLLocalSave(String date) {
  if (SaveSensorValue("s_h1", date, String(map(analogRead(YLPIN1), 0, 4095, 100, 0)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h2", date, String(map(analogRead(YLPIN2), 0, 4095, 100, 0)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h3", date, String(map(analogRead(YLPIN3), 0, 4095, 100, 0)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h4", date, String(map(analogRead(YLPIN4), 0, 4095, 100, 0)))) {
    NoSD();
  }
}
void YLUpToUbi() {
  float sh1 = map(analogRead(YLPIN1), 0, 4095, 100, 0);
  float sh2 = map(analogRead(YLPIN2), 0, 4095, 100, 0);
  float sh3 = map(analogRead(YLPIN3), 0, 4095, 100, 0);
  float sh4 = map(analogRead(YLPIN4), 0, 4095, 100, 0);
  ubidots.add("hs1", sh1);
  ubidots.add("hs2", sh2);
  ubidots.add("hs3", sh3);
  ubidots.add("hs4", sh4);
  ubidots.add("P_H_S", S_HUM);
  ubidots.publish(DEVICE_LABEL);
}