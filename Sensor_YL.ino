void SetupYL() {
  pinMode(S_HUMMIN_Ctrl, OUTPUT);
  pinMode(S_HUMMAX_Ctrl, OUTPUT);
  digitalWrite(S_HUMMIN_Ctrl, VLOW);
  digitalWrite(S_HUMMAX_Ctrl, VLOW);
}
void YLCheck() {
  float sh1 = map(analogRead(YLPIN1), humedadAire, humedadAgua, 0, 100);
  float sh2 = map(analogRead(YLPIN2), humedadAire1, humedadAgua1, 0, 100);
  float sh3 = map(analogRead(YLPIN3), humedadAire2, humedadAgua2, 0, 100);
  float sh4 = map(analogRead(YLPIN4), humedadAire3, humedadAgua3, 0, 100);
  float sum = 0;
  int counter = 0;
  if (sh1 != -117) {
    sum += sh1;
    counter++;
  }
  if (sh2 != -117) {
    sum += sh2;
    counter++;
  }
  if (sh3 != -117) {
    sum += sh3;
    counter++;
  }
  if (sh4 != -117) {
    sum += sh4;
    counter++;
  }
  S_HUM = (sum / counter) ? sum / counter : 0;
  
  //Subir humedad
  if(S_HUMMIN_Ctrl && !dROP(S_HUMMIN_Ctrl) && S_HUM <= S_HUMMIN){
    digitalWrite(S_HUMMIN_Ctrl, VHIGH);
    DataLogger("Control para subir humedad en sustrato", 0);
  }else
  if(S_HUMMIN_Ctrl && dROP(S_HUMMIN_Ctrl) && S_HUM >= (S_HUMIDEAL - 1)){
    digitalWrite(S_HUMMIN_Ctrl, VLOW);
    DataLogger("Apagado control para subir humedad en sustrato", 0);
  }
  //Bajar humedad
  /*
  if(!dROP(S_HUMMAX_Ctrl) && S_HUM >= S_HUMMAX){
    digitalWrite(S_HUMMAX_Ctrl, VHIGH);
    DataLogger("Control para bajar humedad en sustrato", 0);
  }else
  if(dROP(S_HUMMAX_Ctrl) && S_HUM <= (S_HUMIDEAL + 1)){
    digitalWrite(S_HUMMAX_Ctrl, VLOW);
    DataLogger("Apagado control para bajar humedad en sustrato", 0);
  }*/
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
  float sh1 = map(analogRead(YLPIN1), humedadAire, humedadAgua, 0, 100);
  float sh2 = map(analogRead(YLPIN2), humedadAire1, humedadAgua1, 0, 100);
  float sh3 = map(analogRead(YLPIN3), humedadAire2, humedadAgua2, 0, 100);
  float sh4 = map(analogRead(YLPIN4), humedadAire3, humedadAgua3, 0, 100);
  if (SaveSensorValue("s_h1", date, (sh1 == 100 ? "n" : String(sh1)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h2", date, (sh2 == 100 ? "n" : String(sh2)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h3", date, (sh3 == 100 ? "n" : String(sh3)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h4", date, (sh4 == 100 ? "n" : String(sh4)))) {
    NoSD();
  }
}
void YLUpToUbi(String DATE) {
  float sh1 = map(analogRead(YLPIN1), humedadAire, humedadAgua, 0, 100);
  float sh2 = map(analogRead(YLPIN2), humedadAire1, humedadAgua1, 0, 100);
  float sh3 = map(analogRead(YLPIN3), humedadAire2, humedadAgua2, 0, 100);
  float sh4 = map(analogRead(YLPIN4), humedadAire3, humedadAgua3, 0, 100);
  float sum = 0;
  int counter = 0;
  if (sh1 != -117) {
    sum += sh1;
    counter++;
    if (ubidots.connected()) {
      ubidots.add("hs1", sh1);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('hs1', '" + DATE + "','" + sh1 + "', 'no')").c_str());
    }
  }
  if (sh2 != -117) {
    sum += sh2;
    counter++;
    if (ubidots.connected()) {
      ubidots.add("hs2", sh2);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('hs2', '" + DATE + "','" + sh2 + "', 'no')").c_str());
    }
  }
  if (sh3 != -117) {
    sum += sh3;
    counter++;
    if (ubidots.connected()) {
      ubidots.add("hs3", sh3);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('hs3', '" + DATE + "','" + sh3 + "', 'no')").c_str());
    }
  }
  if (sh4 != -117) {
    sum += sh4;
    counter++;
    if (ubidots.connected()) {
      ubidots.add("hs4", sh4);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('hs4', '" + DATE + "','" + sh4 + "', 'no')").c_str());
    }
  }
  //DataLogger("Enviando sensor de humedad en sustrtato en la nube ", 0 );
  S_HUM = (sum / counter) ? sum / counter : 0;
  if (ubidots.connected()) {
    ubidots.add("P_H_S", S_HUM);
    ubidots.publish(DEVICE_LABEL.c_str());
  } else {
    db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('P_H_S', '" + DATE + "','" + S_HUM + "', 'no')").c_str());
  }
}
