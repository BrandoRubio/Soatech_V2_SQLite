#include "RTClib.h"

RTC_DS3231 rtc;

void SetupRTC() {
<<<<<<< HEAD
  //dataLog("Inicializamos modulo RTC", 0 );
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    
=======
  SPI.begin();
  SD_MMC.begin();
  SD.begin();
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
>>>>>>> 5d2c068e0ecc0cace35c9efcc28ae04cbddcc677
    Serial.flush();
    while (1) delay(10);
  }
  DataLogger("Iniciando......", 0);
  if (rtc.lostPower()) {
    DataLogger("Batería del RTC agotada", 1);
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  configTime(0, 0, "pool.ntp.org");
}
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}
void updateRTC() {
  unsigned long epochTime;
  epochTime = getTime();
  DateTime time = rtc.now();
  if (epochTime) {
    Serial.println(String("DateTime::TIMESTAMP_FULL:\t") + time.timestamp(DateTime::TIMESTAMP_FULL));
    rtc.adjust(DateTime(epochTime - 21600));  //Actualizamos la fecha desde NTP server
    DataLogger("Actualizada la hora del reloj " + time.timestamp(DateTime::TIMESTAMP_FULL), 0);
  } else {
    epochTime = getTime();
    if (epochTime) {
      Serial.println(String("DateTime::TIMESTAMP_FULL:\t") + time.timestamp(DateTime::TIMESTAMP_FULL));
      Serial.println(time.unixtime());
      DataLogger("Actualizada la hora del reloj " + time.timestamp(DateTime::TIMESTAMP_FULL), 0);
    } else {
      DataLogger("No se pudo obtener la hora desde la red", 1);
      Serial.println("No se pudo actualizar la hora debido a la red.");
    }
  }
}
