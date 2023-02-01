int analogBuffer[30];  // store the analog value in the array, read from ADC
int analogBufferTemp[30];
int analogBufferIndex = 0;
int copyIndex = 0;

float averageVoltage = 0;
float tdsValue = 0;
float temperature = 25;

// median filtering algorithm
int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0) {
    bTemp = bTab[(iFilterLen - 1) / 2];
  } else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}

void setupConductivity() {
  pinMode(CONDPIN, INPUT);
  //pinMode(CONDMINCONTROL, OUTPUT);
  //digitalWrite(CONDMINCONTROL, VLOW);
}
void CondCheck(){
  getCond();/*
  //Subir conductividad
  if (CONDMINCONTROL && digitalRead(CONDMINCONTROL) == VLOW && COND < (CONDMIN + 1)) {
    DataLogger("Control para subir conductividad", 0);
    digitalWrite(CONDMINCONTROL, HIGH);
  } else if (CONDMINCONTROL && digitalRead(CONDMINCONTROL) == VHIGH && COND >= (CONDIDEAL - 1)) {
    DataLogger("Apagado control para subir conductividad en sustrato", 0);
    digitalWrite(CONDMINCONTROL, VLOW);
  }
  //Bajar conductividad
  if (CONDMAXCONTROL && digitalRead(CONDMAXCONTROL) == VLOW && COND > (CONDMAX - 1)) {  //Cuando la conductividad sube a la máxima ACCIONA control
    DataLogger("Control para bajar conductividad", 0);
    digitalWrite(CONDMAXCONTROL, HIGH);
  } else if (CONDMAXCONTROL && digitalRead(CONDMAXCONTROL) == VHIGH && COND <= (CONDIDEAL + 1)) {
    DataLogger("Apagado control para bajar conductividad", 0);
    digitalWrite(CONDMAXCONTROL, VLOW);
  }*/
  if (alternadorLCD == N_COND) {
    lcd.setCursor(0, 0);
    lcd.print("Conductividad:" + String(COND) + "    ");
    lcd.setCursor(0, 1);
    lcd.print("                    ");
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
  }  
}
void CondLocalSave(String date) {
  if (SaveSensorValue("cond", date, (COND == 0 ? "NULL" : String(COND)))) {
    NoSD();
  }
}
void CondUpToUbi(String DATE) {
  if (ubidots.connected()) {
    ubidots.add("conductividad", COND);
    ubidots.publish(DEVICE_LABEL.c_str());
  } else {
    db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('conductividad', '" + DATE + "','" + COND + "', 'no')").c_str());
  }
}
void getCond() {
  static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 40U)  //every 40 milliseconds,read the analog value from the ADC
  {
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(CONDPIN);  //read the analog value and store into the buffer
    analogBufferIndex++;
    if (analogBufferIndex == 30)
      analogBufferIndex = 0;
  }
  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 800U) {
    float tdsValue = 0;
    printTimepoint = millis();
    for (copyIndex = 0; copyIndex < 30; copyIndex++)
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
    averageVoltage = getMedianNum(analogBufferTemp, 30) * (float)5.0 / 4096.0;                                                                                                        // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);                                                                                                                //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVolatge = averageVoltage / compensationCoefficient;                                                                                                             //temperature compensation
    COND = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5;  //convert voltage value to tds value
    
  }
}
