float sh1, sh2, sh3, sh4, sh5, sh6, sh7, sh8, sum;
int counter;
void SetupYL() {
  pinMode(S_HUMMIN_Ctrl, OUTPUT);
  pinMode(S_HUMMAX_Ctrl, OUTPUT);
  digitalWrite(S_HUMMIN_Ctrl, VLOW);
  digitalWrite(S_HUMMAX_Ctrl, VLOW);
}
void YLCheck() {
  sh1 = analogRead(YLPIN1);
  sh2 = analogRead(YLPIN2);
  sh3 = analogRead(YLPIN3);
  sh4 = analogRead(YLPIN4);
  sh5 = map(analogRead(YLPIN1), humedadAire, humedadAgua, 0, 100);
  sh6 = map(analogRead(YLPIN2), humedadAire1, humedadAgua1, 0, 100);
  sh7 = map(analogRead(YLPIN3), humedadAire2, humedadAgua2, 0, 100);
  sh8 = map(analogRead(YLPIN4), humedadAire3, humedadAgua3, 0, 100);
  sum = 0;
  counter = 0;
  if (sh1 != 0) {
    sum += sh5;
    counter++;
  }
  if (sh1 == 0) {
    sh5 = 0;
    counter++;
  }
  if (sh2 != 0) {
    sum += sh6;
    counter++;
  }
  if (sh2 == 0) {
    sh6 = 0;
  }
  if (sh3 != 0) {
    sum += sh7;
    counter++;
  }
  if (sh3 == 0) {
    sh7 = 0;
  }
  if (sh4 != 0) {
    sum += sh8;
    counter++;
  }
  if (sh4 == 0) {
    sh8 = 0;
  }
  S_HUM = (sum / counter) ? sum / counter : 0;
  
  //Subir humedad
  if(S_HUMMIN_Ctrl && digitalRead(S_HUMMIN_Ctrl) == VLOW && S_HUM <= S_HUMMIN){
    digitalWrite(S_HUMMIN_Ctrl, VHIGH);
    DataLogger("Control para subir humedad en sustrato", 0);
  }else
  if(S_HUMMIN_Ctrl && digitalRead(S_HUMMIN_Ctrl) == VHIGH && S_HUM >= (S_HUMIDEAL - 1)){
    digitalWrite(S_HUMMIN_Ctrl, VLOW);
    DataLogger("Apagado control para subir humedad en sustrato", 0);
  }
  //Bajar humedad
  /*
  if(S_HUMMAX_Ctrl && digitalRead(S_HUMMAX_Ctrl) == VLOW && S_HUM >= S_HUMMAX){
    digitalWrite(S_HUMMAX_Ctrl, VHIGH);
    DataLogger("Control para bajar humedad en sustrato", 0);
  }else
  if(S_HUMMAX_Ctrl && digitalRead(S_HUMMAX_Ctrl) == VHIGH && S_HUM <= (S_HUMIDEAL + 1)){
    digitalWrite(S_HUMMAX_Ctrl, VLOW);
    DataLogger("Apagado control para bajar humedad en sustrato", 0);
  }*/
  if (alternadorLCD == NS_H) {
    //Serial.println("Humedad = " + String(sh1) + "  Humedad2= " + String(sh2) + "  Humedad3= " + String(sh3) +"   Humedad4= " + String(sh4) ); 
    lcd.setCursor(0, 0);
    lcd.print("H1:" + String(sh5) + " ");
    lcd.setCursor(9, 0);
    lcd.print(" H2:" + String(sh6));
    lcd.setCursor(0, 1);
    lcd.print("H3:" + String(sh7) + " ");
    lcd.setCursor(9, 1);
    lcd.print(" H4:" + String(sh8));
    lcd.setCursor(0, 2);
    lcd.print("Promedio: " + String(S_HUM) + "   ");
  }
}
void YLLocalSave(String date) {
  sh1 = analogRead(YLPIN1);
  sh2 = analogRead(YLPIN2);
  sh3 = analogRead(YLPIN3);
  sh4 = analogRead(YLPIN4);
  
  sh5 = map(analogRead(YLPIN1), humedadAire, humedadAgua, 0, 100);
  sh6 = map(analogRead(YLPIN2), humedadAire1, humedadAgua1, 0, 100);
  sh7 = map(analogRead(YLPIN3), humedadAire2, humedadAgua2, 0, 100);
  sh8 = map(analogRead(YLPIN4), humedadAire3, humedadAgua3, 0, 100);
  if (SaveSensorValue("s_h1", date, (sh1 == 100 ? "n" : String(sh5)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h2", date, (sh2 == 100 ? "n" : String(sh6)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h3", date, (sh3 == 100 ? "n" : String(sh7)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h4", date, (sh4 == 100 ? "n" : String(sh8)))) {
    NoSD();
  }
}
void YLUpToUbi(String DATE) {
  sh1 = analogRead(YLPIN1);
  sh2 = analogRead(YLPIN2);
  sh3 = analogRead(YLPIN3);
  sh4 = analogRead(YLPIN4);
  
  sh5 = map(analogRead(YLPIN1), humedadAire, humedadAgua, 0, 100);
  sh6 = map(analogRead(YLPIN2), humedadAire1, humedadAgua1, 0, 100);
  sh7 = map(analogRead(YLPIN3), humedadAire2, humedadAgua2, 0, 100);
  sh8 = map(analogRead(YLPIN4), humedadAire3, humedadAgua3, 0, 100);
  sum = 0;
  counter = 0;
  if (sh1 != 0) {
    sum += sh5;
    counter++;
    if (ubidots.connected()) {
      ubidots.add("hs1", sh5);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('hs1', '" + DATE + "','" + sh5 + "', 'no')").c_str());
    }
  }
  if (sh2 != -117) {
    sum += sh6;
    counter++;
    if (ubidots.connected()) {
      ubidots.add("hs2", sh6);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('hs2', '" + DATE + "','" + sh6 + "', 'no')").c_str());
    }
  }
  if (sh3 != -117) {
    sum += sh7;
    counter++;
    if (ubidots.connected()) {
      ubidots.add("hs3", sh7);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('hs3', '" + DATE + "','" + sh7 + "', 'no')").c_str());
    }
  }
  if (sh4 != -117) {
    sum += sh8;
    counter++;
    if (ubidots.connected()) {
      ubidots.add("hs4", sh8);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('hs4', '" + DATE + "','" + sh8 + "', 'no')").c_str());
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
