#include <OneWire.h>
#include <DallasTemperature.h>

OneWire ourWire(0);                   //Se establece el pin 26 como bus OneWire
DallasTemperature sensors(&ourWire);  //Se declara una variable u objeto para nuestro sensor

void SetupDS18() {
  ourWire.begin(DS18PIN);
  sensors.begin();  //Se inicia el sensor
}
void DS18Check() {
  sensors.requestTemperatures();  //Se envía el comando para leer la temperatura
  //dataLog("Incializando sensor de temperatura en sustrato", 0 );
  float temp1 = sensors.getTempCByIndex(0);  //Se obtiene la temperatura en ºC
  float temp2 = sensors.getTempCByIndex(1);  //Se obtiene la temperatura en ºC
  float temp3 = sensors.getTempCByIndex(2);  //Se obtiene la temperatura en ºC
  float temp4 = sensors.getTempCByIndex(3);  //Se obtiene la temperatura en ºC
  float sum = 0;
  int counter = 0;
  if (temp1 != -127) {
    sum += temp1;
    //DataLogger("Inicializando primer sensor de sustrato", 0 );
    counter++;
  }
  if (temp2 != -127) {
    sum += temp2;
    //DataLogger("Inicializando segunda sensor de sustrato", 0 );
    counter++;
  }
  if (temp3 != -127) {
    sum += temp3;
    //DataLogger("Inicializando tercer sensor de sustrato", 0 );
    counter++;
  }
  if (temp4 != -127) {
    sum += temp4;
    //DataLogger("Inicializando cuarto sensor de sustrato", 0 );
    counter++;
  }
  S_TEMP = (sum / counter) ? sum / counter : 0;
  //subir temperatura en suatrato
  if (S_TEMPMIN_C && !dROP(S_TEMPMIN_C) && S_TEMP <= S_TEMPIDEAL) {
    DataLogger("Control para subir la temperatura en sustrato", 0);
    digitalWrite(S_TEMPMIN_C, VHIGH);
  } else if (S_TEMPMIN_C && dROP(S_TEMPMIN_C) && S_TEMP >= (S_TEMPIDEAL - 1)) {
    DataLogger("Apagado control para subir temperatura en sustrato", 0);
    digitalWrite(S_TEMPMIN_C, VLOW);
  }
  //bajar temperatura en suatrato
  if (S_TEMPMAX_C && !dROP(S_TEMPMAX_C) && S_TEMP <= S_TEMPIDEAL) {
    DataLogger("Control para bajar la temperatura en sustrato", 0);
    digitalWrite(S_TEMPMAX_C, VHIGH);
  } else if (S_TEMPMAX_C && dROP(S_TEMPMAX_C) && S_TEMP <= (S_TEMPIDEAL + 1)) {
    DataLogger("Apagado control para bajar temperatura en sustrato", 0);
    digitalWrite(S_TEMPMAX_C, VLOW);
  }
  if (alternadorLCD == NS_T) {
    lcd.setCursor(0, 0);
    lcd.print("T1:" + String(temp1) + " ");
    lcd.setCursor(9, 0);
    lcd.print(" T2:" + String(temp2));
    lcd.setCursor(0, 1);
    lcd.print("T3:" + String(temp3) + " ");
    lcd.setCursor(9, 1);
    lcd.print(" T4:" + String(temp4));
    lcd.setCursor(0, 2);
    lcd.print("Promedio: " + String(S_TEMP) + "   ");
  }
}

void DS18LocalSave(String date) {
  //DataLogger("Guardando datos de Temperatura en micro SD", 0 );
  sensors.requestTemperatures();             //Se envía el comando para leer la temperatura
  float temp1 = sensors.getTempCByIndex(0);  //Se obtiene la temperatura en ºC
  float temp2 = sensors.getTempCByIndex(1);  //Se obtiene la temperatura en ºC
  float temp3 = sensors.getTempCByIndex(2);  //Se obtiene la temperatura en ºC
  float temp4 = sensors.getTempCByIndex(3);  //Se obtiene la temperatura en ºC;
  if (SaveSensorValue("s_t1", date, (temp1 == -127 ? "n" : String(temp1)))) {
    NoSD();
  }
  if (SaveSensorValue("s_t2", date, (temp2 == -127 ? "n" : String(temp2)))) {
    NoSD();
  }
  if (SaveSensorValue("s_t3", date, (temp3 == -127 ? "n" : String(temp3)))) {
    NoSD();
  }
  if (SaveSensorValue("s_t4", date, (temp4 == -127 ? "n" : String(temp4)))) {
    NoSD();
  }
}

void DS18UpToUbi(String DATE) {
  sensors.requestTemperatures();             //Se envía el comando para leer la temperatura
  float temp1 = sensors.getTempCByIndex(0);  //Se obtiene la temperatura en ºC
  float temp2 = sensors.getTempCByIndex(1);  //Se obtiene la temperatura en ºC
  float temp3 = sensors.getTempCByIndex(2);  //Se obtiene la temperatura en ºC
  float temp4 = sensors.getTempCByIndex(3);  //Se obtiene la temperatura en ºC
  float sum = 0;
  int counter = 0;
  if (temp1 != -127) {
    sum += temp1;
    counter++;
    if (ubidots.connected()) {
      ubidots.add("ts1", temp1);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('ts1', '" + DATE + "','" + temp1 + "', 'no')").c_str());
    }
  }
  if (temp2 != -127) {
    sum += temp2;
    counter++;
    if (ubidots.connected()) {
      ubidots.add("ts2", temp2);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('ts2', '" + DATE + "','" + temp2 + "', 'no')").c_str());
    }
  }
  if (temp3 != -127) {
    sum += temp3;
    counter++;
    if (ubidots.connected()) {
      ubidots.add("ts3", temp3);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('ts3', '" + DATE + "','" + temp3 + "', 'no')").c_str());
    }
  }
  if (temp4 != -127) {
    sum += temp4;
    counter++;
    if (ubidots.connected()) {
      ubidots.add("ts4", temp4);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('ts4', '" + DATE + "','" + temp4 + "', 'no')").c_str());
    }
  }
  S_TEMP = (sum / counter) ? sum / counter : 0;
  if (ubidots.connected()) {
    ubidots.add("P_T_S", S_TEMP);
    ubidots.publish(DEVICE_LABEL.c_str());
  } else {
    db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('P_T_S', '" + DATE + "','" + S_TEMP + "', 'no')").c_str());
  }
  //DataLogger("Cargando datos de temperatura en sustrato en la nube", 0 );
}
