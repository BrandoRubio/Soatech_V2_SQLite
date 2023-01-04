#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter;

void SetupLUM() {
  Wire.begin();
  lightMeter.begin();
}
void LUMCheck() {
  lux = lightMeter.readLightLevel();
  if (alternadorLCD == N_LUM) {
    lcd.setCursor(0, 0);
    lcd.print("Luminosidad:" + String(lux) + " ");
    lcd.setCursor(0, 1);
    lcd.print("                        ");
    lcd.setCursor(0, 2);
    lcd.print("                        ");
  }
}

void LUMLocalSave(String date) {
  if (SaveSensorValue("luminosidad", date, (lux == -2 ? "n" : String(lux)))) {
    NoSD();
  }
}

void LUMUpToUbi(String DATE) {
  if (lux != -2) {
    if (ubidots.connected()) {
      ubidots.add("luminosidad", lux);
      ubidots.publish(DEVICE_LABEL);
    } else {
      db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('luminosidad', '" + DATE + "','" + lux + "', 'no')").c_str());
    }
  }
}
