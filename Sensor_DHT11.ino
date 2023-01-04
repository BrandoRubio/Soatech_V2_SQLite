#include "DHT.h"
#define DHTTipo DHT11
DHT dht1(0, DHTTipo);
DHT dht2(0, DHTTipo);
DHT dht3(0, DHTTipo);
DHT dht4(0, DHTTipo);
//DHT dht5(0, DHTTipo);
//DHT dht6(0, DHTTipo);

void SetupDHT(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) {
  dht1._pin = pin1;
  dht2._pin = pin2;
  dht3._pin = pin3;
  dht4._pin = pin4;
  //dht5._pin = 12;
  //dht6._pin = 11;
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  //dht5.begin();
  //dht6.begin();
  pinMode(TEMPMINCONTROL, OUTPUT);
  pinMode(TEMPMAXCONTROL, OUTPUT);
  digitalWrite(TEMPMINCONTROL, LOW);
  digitalWrite(TEMPMAXCONTROL, LOW);
}

void DHT11Check() {
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();
  float h3 = dht3.readHumidity();
  float t3 = dht3.readTemperature();
  float h4 = dht4.readHumidity();
  float t4 = dht4.readTemperature();
  float sumTemp = 0;
  float sumHum = 0;
  float counter = 0;
  if (isnan(t1)) {
    h1 = dht1.readHumidity();
    t1 = dht1.readTemperature();
    if (isnan(t1)) {
      t1 = 0;
      h1 = 0;
    }
  } else {
    sumTemp = sumTemp + t1;
    sumHum = sumHum + h1;
    counter++;
  }
  if (isnan(t2)) {
    h2 = dht1.readHumidity();
    t2 = dht1.readTemperature();
    if (isnan(t2)) {
      t2 = 0;
      h2 = 0;
    }
  } else {
    sumTemp = sumTemp + t2;
    sumHum = sumHum + h2;
    counter++;
  }
  if (isnan(t3)) {
    h3 = dht3.readHumidity();
    t3 = dht3.readTemperature();
    if (isnan(t3)) {
      t3 = 0;
      h3 = 0;
    }
  } else {
    sumTemp = sumTemp + t3;
    sumHum = sumHum + h3;
    counter++;
  }
  if (isnan(t4)) {
    h4 = dht1.readHumidity();
    t4 = dht1.readTemperature();
    if (isnan(t4)) {
      //Serial.println("e4");
      t4 = 0;
      h4 = 0;
    }
  } else {
    sumTemp = sumTemp + t4;
    sumHum = sumHum + h4;
    counter++;
  }
  TEMP = sumTemp / counter;
  HUM = sumHum / counter;
  if (TEMP < (TEMPMIN + 1) && !STDHTMIN) {  //Cuando la temperatura baja a la mínima + 1 ACCIONA control
    DataLogger("Control para subir temperatura", 0);
    digitalWrite(TEMPMINCONTROL, HIGH);
    STDHTMIN = true;
    STDHT = false;
  }
  if (TEMP > (TEMPMAX - 1) && !STDHTMAX) {  //Cuando la temperatura sube a la máxima - 1 ACCIONA control
    DataLogger("Control para bajar temperatura", 0);
    digitalWrite(TEMPMAXCONTROL, HIGH);
    STDHTMAX = true;
    STDHT = false;
    //>>>>>>> 5d2c068e0ecc0cace35c9efcc28ae04cbddcc677
  }
  if (TEMP >= (TEMPIDEAL - 1) && TEMP <= (TEMPIDEAL + 1) && !STDHT) {  //Cuando la temperatura se encuentra estable desactiva todo el control
    DataLogger("Apagamos todos los controles de temperatura", 0);
    digitalWrite(TEMPMINCONTROL, LOW);
    digitalWrite(TEMPMAXCONTROL, LOW);
    STDHT = true;
    STDHTMAX = false;
    STDHTMIN = false;
    //>>>>>>> 5d2c068e0ecc0cace35c9efcc28ae04cbddcc677
  }
  if (alternadorLCD == N_DHT) {
    lcd.setCursor(0, 0);
    lcd.print("Temperatura:" + String(TEMP) + "   ");
    lcd.setCursor(0, 1);
    lcd.print("Humedad:    " + String(HUM) + "   ");
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
  }
}

void DHT11LocalSave(String date) {
  DataLogger("Guardando datos de temperatura y humedad relativa en Micro SD", 0);
  if (SaveSensorValue("temperatura", date, (isnan(TEMP) ? "NULL" : String(TEMP)))) {
    NoSD();
  }
  if (SaveSensorValue("humedad", date, (isnan(HUM) ? "NULL" : String(HUM)))) {
    NoSD();
  }
}

void DHT11UpToUbi(String DATE) {
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();
  float h3 = dht3.readHumidity();
  float t3 = dht3.readTemperature();
  float h4 = dht4.readHumidity();
  float t4 = dht4.readTemperature();
  delay(20);
  if (isnan(t1) || isnan(h1)) {
  } else {
    if (ubidots.connected()) {
      ubidots.add("t1", t1);
      ubidots.add("h1", h1);
    } else {
      Serial.print("~");
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('t1', '" + DATE + "','" + t1 + "', 'no')").c_str());
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('h1', '" + DATE + "','" + h1 + "', 'no')").c_str());
    }
  }
  if (isnan(t2) || isnan(h2)) {
  } else {
    if (ubidots.connected()) {
      ubidots.add("t2", t2);
      ubidots.add("h2", h2);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('t2', '" + DATE + "','" + t2 + "', 'no')").c_str());
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('h2', '" + DATE + "','" + h2 + "', 'no')").c_str());
    }
  }
  if (isnan(t3) || isnan(h3)) {
  } else {
    if (ubidots.connected()) {
      ubidots.add("t3", t3);
      ubidots.add("h3", h3);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('t3', '" + DATE + "','" + t3 + "', 'no')").c_str());
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('h3', '" + DATE + "','" + h3 + "', 'no')").c_str());
    }
  }
  if (isnan(t4) || isnan(h4)) {
  } else {
    if (ubidots.connected()) {
      ubidots.add("t4", t4);
      ubidots.add("h4", h4);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('t4', '" + DATE + "','" + t4 + "', 'no')").c_str());
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('h4', '" + DATE + "','" + h4 + "', 'no')").c_str());
    }
  }
  if (ubidots.connected()) {
    DataLogger("Subiendo datos de temperatura y humedad relativa a la nube", 0);
    ubidots.add("Temperatura", TEMP);
    ubidots.add("Humedad", HUM);
  } else {
    db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('Temperatura', '" + DATE + "','" + TEMP + "', 'no')").c_str());
    db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('Humedad', '" + DATE + "','" + HUM + "', 'no')").c_str());
  }
  ubidots.publish(DEVICE_LABEL);
}
