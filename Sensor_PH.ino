//float calibration = 15.75; //valor de b para calibración sensor pH //9
int buf[10],temp11;

void PHCheck(){
   PH= getPH();
  /*if (PH >= (PHIDEAL - 1) && PH <= (PHIDEAL + 1) && !STPH) {
    DataLogger("Apagamos todos los controles del PH", 0);
    STPH = true;
    STPHMAX = false;
    STPHMIN = false;
    digitalWrite(PHMINCONTROL, VLOW);
    digitalWrite(PHMAXCONTROL, VLOW);
  }*/
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
    ubidots.publish(DEVICE_LABEL);
  } else {
    db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('ph', '" + DATE + "','" + PH + "', 'no')").c_str());
  }
}
float getPH(){
  for(int i=0;i<10;i++) 
  { 
    buf[i]=analogRead(PHPIN);
    delay(30);
  }
  for(int i=0;i<9;i++)
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp11=buf[i];
        buf[i]=buf[j];
        buf[j]=temp11;
      }
    }
  }
  long int avgValue=0;
  for(int i=2;i<8;i++)
    avgValue+=buf[i];
  float pHVol=((((float)avgValue)*5.0)/4095.0)/6;
  float PH = (-2.9718 * pHVol) + 18.6;
  return(PH);
}
