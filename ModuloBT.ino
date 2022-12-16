#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

void SetupBT() {
  DataLogger("Iniciando Bluetooth para cambio de red", 0);
  SerialBT.begin("Soatech Device");  //Bluetooth device name
  DataLogger("Iniciando base de datos <b>BT</b>", 0);
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println(F("Failed to mount file Serial"));
    return;
  }
  sqlite3_initialize();
  db_open();
  while (true) {
    loopBT();
  }
}

void loopBT() {
  if (SerialBT.available()) {
    String ssidpass = SerialBT.readStringUntil('\n');
    ssidpass.remove(ssidpass.length() - 1, 1);
    int s = ssidpass.indexOf(',');
    WIFISSID = ssidpass.substring(0, s);
    PASSWORD = ssidpass.substring(s + 1);
    if (WIFISSID) {
      DataLogger("Guardando nueva red: " + WIFISSID + "* vía Bluetooth", 0);
      String query = "UPDATE device SET network = '" + WIFISSID + "', password = '" + PASSWORD + "' where id = 1";
      int r = db_exec(query.c_str());
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" Red y contraseña  ");
      lcd.setCursor(0, 2);
      lcd.print("     recibidos.    ");
      delay(3000);
      DataLogger("Red y contraseña actualizada vía Bluetooth", r);
      ESP.restart();
    }
  }
  delay(20);
}