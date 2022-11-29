#include <OneWire.h>
#include <DallasTemperature.h>

OneWire ourWire(0);                   //Se establece el pin 26 como bus OneWire
DallasTemperature sensors(&ourWire);  //Se declara una variable u objeto para nuestro sensor

void SetupDS18() {
  ourWire.begin(DS18PIN);
  sensors.begin();  //Se inicia el sensor
}
void DS18Check() {
  sensors.requestTemperatures();             //Se envía el comando para leer la temperatura
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
  //DataLogger("Calculando promedio de temperatura en sustrato", 0 );
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
  DataLogger("Guardando datos de Temperatura en micro SD", 0 );
  sensors.requestTemperatures();             //Se envía el comando para leer la temperatura
  float temp1 = sensors.getTempCByIndex(0);  //Se obtiene la temperatura en ºC
  float temp2 = sensors.getTempCByIndex(1);  //Se obtiene la temperatura en ºC
  float temp3 = sensors.getTempCByIndex(2);  //Se obtiene la temperatura en ºC
  float temp4 = sensors.getTempCByIndex(3);  //Se obtiene la temperatura en ºC;
  if (SaveSensorValue("s_t1", date, (temp1 == -127 ? "NULL" : String(temp1)))) {
    NoSD();
  }
  if (SaveSensorValue("s_t2", date, (temp2 == -127 ? "NULL" : String(temp2)))) {
    NoSD();
  }
  if (SaveSensorValue("s_t3", date, (temp3 == -127 ? "NULL" : String(temp3)))) {
    NoSD();
  }
  if (SaveSensorValue("s_t4", date, (temp4 == -127 ? "NULL" : String(temp4)))) {
    NoSD();
  }
}

void DS18UpToUbi() {
  sensors.requestTemperatures();             //Se envía el comando para leer la temperatura
  float temp1 = sensors.getTempCByIndex(0);  //Se obtiene la temperatura en ºC
  float temp2 = sensors.getTempCByIndex(1);  //Se obtiene la temperatura en ºC
  float temp3 = sensors.getTempCByIndex(2);  //Se obtiene la temperatura en ºC
  float temp4 = sensors.getTempCByIndex(3);  //Se obtiene la temperatura en ºC
  float sum = 0;
  int counter = 0;
  if (temp1 != -127) {
    ubidots.add("ts1", temp1);
    sum += temp1;
    counter++;
  }
  if (temp2 != -127) {
    ubidots.add("ts2", temp2);
    sum += temp2;
    counter++;
  }
  if (temp3 != -127) {
    ubidots.add("ts3", temp3);
    sum += temp3;
    counter++;
  }
  if (temp4 != -127) {
    ubidots.add("ts4", temp4);
    sum += temp4;
    counter++;
  }
  DataLogger("Cargando datos de temperatura en sustrato en la nube", 0 );
  S_TEMP = (sum / counter) ? sum / counter : 0;
  ubidots.add("P_T_S", S_TEMP);
  ubidots.publish(DEVICE_LABEL);
}
