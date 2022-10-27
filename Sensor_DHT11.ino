#include "DHT.h"
#define DHTTipo DHT11
DHT dht1(0, DHTTipo);
DHT dht2(0, DHTTipo);
DHT dht3(0, DHTTipo);
DHT dht4(0, DHTTipo);
DHT dht5(0, DHTTipo);
DHT dht6(0, DHTTipo);

void SetupDHT(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) {
  dht1._pin = pin1;
  dht2._pin = pin2;
  dht3._pin = pin3;
  dht4._pin = pin4;
  dht5._pin = 12;
  dht6._pin = 11;
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();
  dht6.begin();
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
  float h5 = dht4.readHumidity();
  float t5 = dht4.readTemperature();
  float h6 = dht4.readHumidity();
  float t6 = dht4.readTemperature();
  Serial.println(h1);
  Serial.println(t1);
  Serial.println(h2);
  Serial.println(t2);
  Serial.println(h3);
  Serial.println(t3);
  Serial.println(h4);
  Serial.println(t4);
  Serial.println(h5);
  Serial.println(t5);
  Serial.println(h6);
  Serial.println(t6);
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
    if (isnan(t1)) {
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
    if (isnan(t1)) {
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
  lcd.setCursor(0, 0);
  lcd.print("Temperatura:" + String(TEMP));
  lcd.setCursor(0, 1);
  lcd.print("Humedad:    " + String(HUM));
}

void DHT11LocalSave(String date) {
  if (SaveSensorValue("temperatura", date, (isnan(TEMP) ? "0" : String(TEMP)))) {
    NoSD();
  }
  if (SaveSensorValue("humedad", date, (isnan(HUM) ? "0" : String(HUM)))) {
    NoSD();
  }
}