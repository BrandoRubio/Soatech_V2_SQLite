#include <MQUnifiedsensor.h>

#define placa "ESP-32"
#define Voltage_Resolution 3.3
#define type1 " MQ-135 "
#define ADC_Bit_Resolution 12
#define RatioMQ135CleanAir 3.6
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, CO2PIN, type1);

void SetupCO2() {
  MQ135.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ135.init();
  //Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ135.update();  // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0 / 10);
  //Serial.println("  done!.");
  if (isinf(calcR0) || calcR0 == 0) {
    lcd.setCursor(0, 3);
    lcd.print("Error en sensor de CO2");
    CO2_ACTIVE = false;
  }
  /*****************************  MQ CAlibration **************************/
  MQ135.serialDebug(false);
}

void CO2Check() {
  MQ135.update();
  MQ135.setA(102.2);
  MQ135.setB(-2.473);
  NH3 = MQ135.readSensor();
  MQ135.setA(110.47);
  MQ135.setB(-2.862);
  CO2 = MQ135.readSensor() * 1000;
  if (alternadorLCD == N_CO2) {
    lcd.setCursor(0, 0);
    lcd.print("CO2:" + String(CO2) + "                ");
    lcd.setCursor(0, 1);
    lcd.print("                        ");
    lcd.setCursor(0, 2);
    lcd.print("                        ");
    lcd.setCursor(0, 3);
  }
}
void CO2LocalSave(String date) {
  MQ135.update();
  MQ135.setA(102.2);
  MQ135.setB(-2.473);
  NH3 = MQ135.readSensor();
  MQ135.setA(110.47);
  MQ135.setB(-2.862);
  CO2 = MQ135.readSensor() * 1000;
  if (SaveSensorValue("CO2", date, (CO2 == 0 ? "n" : String(CO2)))) {
    NoSD();
  }
}
void CO2UpToUbi(String DATE) {
  MQ135.update();
  MQ135.setA(102.2);
  MQ135.setB(-2.473);
  NH3 = MQ135.readSensor();
  MQ135.setA(110.47);
  MQ135.setB(-2.862);
  CO2 = MQ135.readSensor() * 1000;
  if (CO2 != 0) {
    if (ubidots.connected()) {
      ubidots.add("CO2", CO2);
      ubidots.publish(DEVICE_LABEL);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('CO2', '" + DATE + "','" + CO2 + "', 'no')").c_str());
    }
  }
  ubidots.publish(DEVICE_LABEL);
}
