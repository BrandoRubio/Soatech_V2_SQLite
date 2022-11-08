#include "RTClib.h"

RTC_DS3231 rtc;

void SetupRTC() {
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  
  if (rtc.lostPower()) {
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
void updateRTC(){
  unsigned long epochTime;
  epochTime = getTime();
  DateTime time = rtc.now();
  Serial.println(String("DateTime::TIMESTAMP_FULL:\t") + time.timestamp(DateTime::TIMESTAMP_FULL));
  rtc.adjust(DateTime(epochTime - 21600));  //Actualizamos la fecha desde NTP server
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
}
