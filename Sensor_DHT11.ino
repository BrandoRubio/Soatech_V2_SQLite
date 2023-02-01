#include "DHT.h"
#define DHTTipo DHT11
DHT dht1(0, DHTTipo);
DHT dht2(0, DHTTipo);
DHT dht3(0, DHTTipo);
DHT dht4(0, DHTTipo);
DHT dht5(0, DHTTipo);
DHT dht6(0, DHTTipo);
DHT dht7(0, DHTTipo);

void SetupDHT() {
  dht1._pin = DHTPIN1;
  dht2._pin = DHTPIN2;
  dht3._pin = DHTPIN3;
  dht4._pin = DHTPIN4;
  dht5._pin = DHTPIN5;
  dht6._pin = DHTPIN6;
  dht7._pin = DHTPIN7;
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();
  dht6.begin();
  dht7.begin();
  pinMode(TEMPMINCONTROL, OUTPUT);
  pinMode(TEMPMAXCONTROL, OUTPUT);
  pinMode(TEMPFAN, OUTPUT);
  digitalWrite(TEMPMINCONTROL, VLOW);
  digitalWrite(TEMPMAXCONTROL, VLOW);
  digitalWrite(TEMPFAN, VLOW);
  DHTPIN1, DHTPIN2, DHTPIN3, DHTPIN4, DHTPIN5, DHTPIN6 = 0;
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
  float h5 = dht5.readHumidity();
  float t5 = dht5.readTemperature();
  float h6 = dht6.readHumidity();
  float t6 = dht6.readTemperature();
  float h7 = dht7.readHumidity();
  float t7 = dht7.readTemperature();
  float sumTemp = 0;
  float sumHum = 0;
  float counter = 0;
  int cs = 0;        //conteo superior
  float ss, ps = 0;  // sumatoria y promedio parte superior
  int ci = 0;        //conteo inferior
  float si, pi = 0;  // sumatoria y promedio parte inferior
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
    ss = ss + t1;
    cs++;
  }
  if (isnan(t2)) {
    h2 = dht2.readHumidity();
    t2 = dht2.readTemperature();
    if (isnan(t2)) {
      t2 = 0;
      h2 = 0;
    }
  } else {
    sumTemp = sumTemp + t2;
    sumHum = sumHum + h2;
    counter++;
    ss = ss + t2;
    cs++;
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
    ss = ss + t3;
    cs++;
  }
  if (isnan(t4)) {
    h4 = dht4.readHumidity();
    t4 = dht4.readTemperature();
    if (isnan(t4)) {
      //Serial.println("e4");
      t4 = 0;
      h4 = 0;
    }
  } else {
    sumTemp = sumTemp + t4;
    sumHum = sumHum + h4;
    counter++;
    ss = ss + t4;
    cs++;
  }
  if (isnan(t5)) {
    h5 = dht5.readHumidity();
    t5 = dht5.readTemperature();
    if (isnan(t5)) {
      //Serial.println("e4");
      t5 = 0;
      h5 = 0;
    }
  } else {
    sumTemp = sumTemp + t5;
    sumHum = sumHum + h5;
    counter++;
    si = si + t5;
    ci++;
  }
  if (isnan(t6)) {
    h6 = dht6.readHumidity();
    t6 = dht6.readTemperature();
    if (isnan(t6)) {
      //Serial.println("e4");
      t6 = 0;
      h6 = 0;
    }
  } else {
    sumTemp = sumTemp + t6;
    sumHum = sumHum + h6;
    counter++;
    si = si + t6;
    ci++;
  }
  if (isnan(t7)) {
    h7 = dht7.readHumidity();
    t7 = dht7.readTemperature();
    if (isnan(t7)) {
      //Serial.println("e4");
      t7 = 0;
      h7 = 0;
    }
  }
  TEMP = sumTemp / counter;
  HUM = sumHum / counter;
  ps = ss / cs;
  pi = si / ci;
  if ((ps - pi) >= 3 || (ps - pi) <= -3) {
    if (!digitalRead(TEMPFAN)) {
      digitalWrite(TEMPFAN, VHIGH);
    }else{
      digitalWrite(TEMPFAN, VLOW);
    }
  }
  //Subir temperatura
  if (TEMPMINCONTROL && digitalRead(TEMPMINCONTROL) == VLOW && TEMP < (TEMPMIN + 1)) {  //Cuando la temperatura baja a la mínima ACCIONA control
    DataLogger("Control para subir temperatura", 0);
    digitalWrite(TEMPMINCONTROL, HIGH);
  } else if (TEMPMINCONTROL && digitalRead(TEMPMINCONTROL) == VHIGH && TEMP >= (TEMPIDEAL - 1)) {
    DataLogger("Apagado control para subir temperatura en sustrato", 0);
    digitalWrite(TEMPMINCONTROL, VLOW);
  }
  //Bajar tempertura
  if (TEMPMAXCONTROL && digitalRead(TEMPMAXCONTROL) == VLOW && TEMP > (TEMPMAX - 1)) {  //Cuando la temperatura sube a la máxima ACCIONA control
    DataLogger("Control para bajar temperatura", 0);
    digitalWrite(TEMPMAXCONTROL, HIGH);
  } else if (TEMPMAXCONTROL && digitalRead(TEMPMAXCONTROL) == VHIGH && TEMP <= (TEMPIDEAL + 1)) {
    DataLogger("Apagado control para bajar temperatura", 0);
    digitalWrite(TEMPMAXCONTROL, VLOW);
  }
  //Subir humedad
  if (HUMMINCONTROL && digitalRead(HUMMINCONTROL) == VLOW && HUM < (HUMMIN + 1)) {  //Cuando la humedad baja a la mínima ACCIONA control
    DataLogger("Control para subir humedad", 0);
    digitalWrite(HUMMINCONTROL, HIGH);
  } else if (HUMMINCONTROL && digitalRead(HUMMINCONTROL) == VHIGH && HUM >= (HUMIDEAL - 1)) {
    DataLogger("Apagado control para subir humedad en sustrato", 0);
    digitalWrite(HUMMINCONTROL, VLOW);
  }
  //Bajar humedad
  if (HUMMAXCONTROL && digitalRead(HUMMAXCONTROL) == VLOW && HUM > (HUMMAX - 1)) {  //Cuando la humedad sube a la máxima ACCIONA control
    DataLogger("Control para bajar humedad", 0);
    digitalWrite(HUMMAXCONTROL, HIGH);
  } else if (HUMMAXCONTROL && digitalRead(HUMMAXCONTROL) == VHIGH && HUM <= (HUMIDEAL + 1)) {
    DataLogger("Apagado control para bajar humedad", 0);
    digitalWrite(HUMMAXCONTROL, VLOW);
  }
  
  if (alternadorLCD == N_DHT) {
    lcd.setCursor(0, 0);
    lcd.print("Temperatura:" + String(TEMP) + "   ");
    lcd.setCursor(0, 1);
    lcd.print("Humedad:    " + String(HUM) + "   ");
    if (DHTPIN7) {
      lcd.setCursor(0, 2);
      lcd.print("Tem exterior:" + String(t7));
      lcd.setCursor(0, 3);
      lcd.print("Hum exterior:" + String(h7));
    }
  }
}

void DHT11LocalSave(String date) {
  float h7 = dht7.readHumidity();
  float t7 = dht7.readTemperature();
  if (SaveSensorValue("temperatura", date, (isnan(TEMP) ? "NULL" : String(TEMP)))) {
    NoSD();
  }
  if (SaveSensorValue("tex", date, (isnan(t7) ? "NULL" : String(t7)))) {
    NoSD();
  }
  if (SaveSensorValue("hex", date, (isnan(h7) ? "NULL" : String(h7)))) {
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
  float h5 = dht5.readHumidity();
  float t5 = dht5.readTemperature();
  float h6 = dht6.readHumidity();
  float t6 = dht6.readTemperature();
  float h7 = dht7.readHumidity();
  float t7 = dht7.readTemperature();
  int cs = 0;        //conteo superior
  float ss, ps = 0;  // sumatoria y promedio parte superior
  int ci = 0;        //conteo inferior
  float si, pi = 0;  // sumatoria y promedio parte inferior
  delay(20);
  if (isnan(t1) || isnan(h1)) {
  } else {
    ss = ss + t1;
    cs++;
    if (ubidots.connected()) {
      ubidots.add("t1", t1);
      ubidots.add("h1", h1);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('t1', '" + DATE + "','" + t1 + "', 'no')").c_str());
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('h1', '" + DATE + "','" + h1 + "', 'no')").c_str());
    }
  }
  if (isnan(t2) || isnan(h2)) {
  } else {
    ss = ss + t2;
    cs++;
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
    ss = ss + t3;
    cs++;
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
    ss = ss + t4;
    cs++;
    if (ubidots.connected()) {
      ubidots.add("t4", t4);
      ubidots.add("h4", h4);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('t4', '" + DATE + "','" + t4 + "', 'no')").c_str());
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('h4', '" + DATE + "','" + h4 + "', 'no')").c_str());
    }
  }
  if (isnan(t5) || isnan(h5)) {
  } else {
    si = si + t5;
    ci++;
    if (ubidots.connected()) {
      ubidots.add("t5", t5);
      ubidots.add("h5", h5);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('t5', '" + DATE + "','" + t5 + "', 'no')").c_str());
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('h5', '" + DATE + "','" + h5 + "', 'no')").c_str());
    }
  }
  if (isnan(t6) || isnan(h6)) {
  } else {
    si = si + t6;
    ci++;
    if (ubidots.connected()) {
      ubidots.add("t4", t6);
      ubidots.add("h4", h6);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('t6', '" + DATE + "','" + t6 + "', 'no')").c_str());
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('h6', '" + DATE + "','" + h6 + "', 'no')").c_str());
    }
  }
  if (ubidots.connected()) {
    //DataLogger("Subiendo datos de temperatura y humedad relativa a la nube", 0);
    ubidots.add("Temperatura", TEMP);
    ubidots.add("Humedad", HUM);
  } else {
    db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('Temperatura', '" + DATE + "','" + TEMP + "', 'no')").c_str());
    db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('Humedad', '" + DATE + "','" + HUM + "', 'no')").c_str());
  }
  ubidots.publish(DEVICE_LABEL.c_str());
  if (ubidots.connected()) {
    //DataLogger("Subiendo datos de temperatura y humedad exterior a la nube", 0);
    ubidots.add("tex", t7);
    ubidots.add("hex", h7);
    if (cs) {
      ps = ss / cs;
      ubidots.add("tps", ps);
    }
    if (ci) {
      pi = si / ci;
      ubidots.add("tpi", pi);
    }
    ubidots.publish(DEVICE_LABEL.c_str());
  } else {
    if (t7) {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('tex', '" + DATE + "','" + t7 + "', 'no')").c_str());
    }
    if (h7) {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('hex', '" + DATE + "','" + h7 + "', 'no')").c_str());
    }
    if (cs) {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('tps', '" + DATE + "','" + ps + "', 'no')").c_str());
    }
    if (ci) {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('tpi', '" + DATE + "','" + pi + "', 'no')").c_str());
    }
  }
}
