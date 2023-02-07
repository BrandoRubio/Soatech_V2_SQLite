#include <DFRobot_SCD4X.h>
DFRobot_SCD4X SCD4X(&Wire, /*i2cAddr = */SCD4X_I2C_ADDR);

void SetupCO2() {
  SCD4X.enablePeriodMeasure(SCD4X_STOP_PERIODIC_MEASURE);
  SCD4X.setTempComp(4.0);
  float temp = 0;
  temp = SCD4X.getTempComp();
  SCD4X.setSensorAltitude(540);

  uint16_t altitude = 0;
  altitude = SCD4X.getSensorAltitude();
  SCD4X.enablePeriodMeasure(SCD4X_START_PERIODIC_MEASURE);
  
  /*****************************  MQ CAlibration **************************/
  pinMode(CO2MAXCONTROL, OUTPUT);
  digitalWrite(CO2MAXCONTROL, VLOW);
}

void CO2Check() {
  //subir CO2
  if (CO2MINCONTROL && digitalRead(CO2MINCONTROL) == VLOW && CO2 <= CO2IDEAL) {
    DataLogger("Control para subir el CO2", 0);
    digitalWrite(CO2MINCONTROL, VHIGH);
  } else if (CO2MINCONTROL && digitalRead(CO2MINCONTROL) == VHIGH && CO2 >= (CO2IDEAL - 1)) {
    DataLogger("Apagado control para subir el CO2", 0);
    digitalWrite(CO2MINCONTROL, VLOW);
  }
  //bajar CO2
  if (CO2MAXCONTROL && digitalRead(CO2MAXCONTROL) == VLOW && CO2 >= CO2IDEAL) {
    DataLogger("Control para bajar el CO2", 0);
    digitalWrite(CO2MAXCONTROL, VHIGH);
  } else if (CO2MAXCONTROL && digitalRead(CO2MAXCONTROL) == VHIGH && CO2 <= (CO2IDEAL + 1)) {
    DataLogger("Apagado control para bajar el CO2", 0);
    digitalWrite(CO2MAXCONTROL, VLOW);
  }
   if(SCD4X.getDataReadyStatus()) {
  
    DFRobot_SCD4X::sSensorMeasurement_t data;
    SCD4X.readMeasurement(&data);
    CO2 = data.CO2ppm; 
   }
   if (alternadorLCD == N_CO2) {
    lcd.setCursor(0, 0);
    lcd.print("CO2:" + String(CO2) + "                 ");
    lcd.setCursor(0, 1);
    lcd.print("                        ");
    lcd.setCursor(0, 2);
    lcd.print("                        ");
    lcd.setCursor(0, 3);
    }
  
}
void CO2LocalSave(String date) {
   if(SCD4X.getDataReadyStatus()) {
  
    DFRobot_SCD4X::sSensorMeasurement_t data;
    SCD4X.readMeasurement(&data);
    CO2 = data.CO2ppm;
    if (SaveSensorValue("CO2", date, (CO2 == 0 ? "n" : String(CO2)))) {
      NoSD();
    }
  }
}
void CO2UpToUbi(String DATE) {
   if(SCD4X.getDataReadyStatus()) {
  
    DFRobot_SCD4X::sSensorMeasurement_t data;
    SCD4X.readMeasurement(&data);
    CO2 = data.CO2ppm;
      if (CO2 != 0) {
        if (ubidots.connected()) {
        ubidots.add("CO2", CO2);
        ubidots.publish(DEVICE_LABEL.c_str());
      } else {
        db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('CO2', '" + DATE + "','" + CO2 + "', 'no')").c_str());
    }
  }
 }
}
