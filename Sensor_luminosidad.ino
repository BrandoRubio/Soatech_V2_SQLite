#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter;

void SetupLUM() {
  Wire.begin();
  lightMeter.begin();
}
void LUMCheck() {
  float lux =lightMeter.readLightLevel();
  if (alternadorLCD == NS_LUM) {
    lcd.setCursor(0, 0);
    lcd.print("Luminosidad:" + String(lux) + " ");
    lcd.setCursor(0, 1);
    lcd.print("                        ");
    lcd.setCursor(0, 2);
    lcd.print("                        ");
    lcd.setCursor(0, 3);
  }
}

void LUMLocalSave(String date) {
  //DataLogger("Guardando datos de Temperatura en micro SD", 0 );
  float lux =lightMeter.readLightLevel();
  if (SaveSensorValue("Luminosidad", date, (lux == -2 ? "n" : String(lux)))) {
    NoSD(); }
}

void LUMUpToUbi() {
  float lux =lightMeter.readLightLevel();
  if (lux != -2) {
    ubidots.add("luminosidad", lux);
  }
  ubidots.publish(DEVICE_LABEL);
}
