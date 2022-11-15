void SetupYL() {
  pinMode(S_HUMMIN_Ctrl, OUTPUT);
  pinMode(S_HUMMAX_Ctrl, OUTPUT);
  digitalWrite(S_HUMMIN_Ctrl, VLOW);
  digitalWrite(S_HUMMAX_Ctrl, VLOW);
}
void YLCheck() {
  float sh1 = map(analogRead(YLPIN1), 0, 4095, 100, 0);
  float sh2 = map(analogRead(YLPIN2), 0, 4095, 100, 0);
  float sh3 = map(analogRead(YLPIN3), 0, 4095, 100, 0);
  float sh4 = map(analogRead(YLPIN4), 0, 4095, 100, 0);
  float sum = 0;
  int counter = 0;
  if (sh1 != 100) {
    sum += sh1;
    counter++;
  }
  if (sh2 != 100) {
    sum += sh2;
    counter++;
  }
  if (sh3 != 100) {
    sum += sh3;
    counter++;
  }
  if (sh4 != 100) {
    sum += sh4;
    counter++;
  }
  S_HUM = (sum / counter) ? sum / counter : 0;
  if (S_HUM > S_HUMMAX) {  //controlar humedad mínima
    digitalWrite(S_HUMMIN_Ctrl, VHIGH);
  }
  if (S_HUM >= (S_HUMIDEAL - 1) && S_HUM <= (S_HUMIDEAL + 1)) {  //Apagar controles
    digitalWrite(S_HUMMIN_Ctrl, VLOW);
    digitalWrite(S_HUMMAX_Ctrl, VLOW);
  }
  if (S_HUM < S_HUMMIN) {  //controlar humedad máxima
    digitalWrite(S_HUMMAX_Ctrl, VHIGH);
  }
  if (alternadorLCD == NS_H) {
    lcd.setCursor(0, 0);
    lcd.print("H1:" + String(sh1) + " ");
    lcd.setCursor(9, 0);
    lcd.print(" H2:" + String(sh2));
    lcd.setCursor(0, 1);
    lcd.print("H3:" + String(sh3) + " ");
    lcd.setCursor(9, 1);
    lcd.print(" H4:" + String(sh4));
    lcd.setCursor(0, 2);
    lcd.print("Promedio: " + String(S_HUM) + "   ");
  }
}
void YLLocalSave(String date) {
  float sh1 = map(analogRead(YLPIN1), 0, 4095, 100, 0);  
  float sh2 = map(analogRead(YLPIN2), 0, 4095, 100, 0);  
  float sh3 = map(analogRead(YLPIN3), 0, 4095, 100, 0);  
  float sh4 = map(analogRead(YLPIN4), 0, 4095, 100, 0);  
  if (SaveSensorValue("s_h1", date, (sh1 == 100 ? "NULL" : String(sh1)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h2", date, (sh2 == 100 ? "NULL" : String(sh2)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h3", date, (sh3 == 100 ? "NULL" : String(sh3)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h4", date, (sh4 == 100 ? "NULL" : String(sh4)))) {
    NoSD();
  }
}
void YLUpToUbi() {
  float sh1 = map(analogRead(YLPIN1), 0, 4095, 100, 0);
  float sh2 = map(analogRead(YLPIN2), 0, 4095, 100, 0);
  float sh3 = map(analogRead(YLPIN3), 0, 4095, 100, 0);
  float sh4 = map(analogRead(YLPIN4), 0, 4095, 100, 0);
  float sum = 0;
  int counter = 0;
  if (sh1 != 100) { 
    ubidots.add("hs1", sh1);
    sum += sh1;
    counter++;
  }
  if (sh2 != 100) {
    ubidots.add("hs2", sh2);
    sum += sh2;
    counter++;
  }
  if (sh3 != 100) {
    ubidots.add("hs3", sh3);
    sum += sh3;
    counter++;
  }
  if (sh4 != 100) {
    ubidots.add("hs4", sh4);
    sum += sh4;
    counter++;
  }
  S_HUM = (sum / counter) ? sum / counter : 0;
  ubidots.add("P_H_S", S_HUM);
  ubidots.publish(DEVICE_LABEL);
}