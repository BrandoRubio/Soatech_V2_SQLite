#include "WiFi.h"
void Connect() {
  /*<<<<<<< HEAD
  dataLog("Conectando a red de wifi..... ", 0 );
=======*/
  DataLogger("Conectando a la red " + WIFISSID, 0);
  //>>>>>>> 5d2c068e0ecc0cace35c9efcc28ae04cbddcc677
  desconexionwifi();
  WiFi.begin(WIFISSID.c_str(), PASSWORD.c_str());
  /*lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("    Conectando a  ");
  lcd.setCursor(1, 2);
  lcd.print(WIFISSID);*/

  int contador = 0;
  unsigned long currentMillis = millis();
  while (WiFi.status() != WL_CONNECTED) {
    LocalCheck();
    SaveLocal();
    bool ft = true;
    if (abs(millis() - currentMillis) > 1000) {
      Serial.print(".");
      lcd.setCursor(contador, 3);
      lcd.print(".");
      contador++;
      currentMillis = millis();
    }
    if (contador > 8) {
      lcd.setCursor(0, 3);
      lcd.print("          ");
      WiFi.disconnect();
      contador = 0;
      delay(100);
      WiFi.begin(WIFISSID.c_str(), PASSWORD.c_str());
      lcd.setCursor(18, 3);
      lcd.print(" ");
    }
  }
  SetupServer();
  ubidots.setup();
  lcd.setCursor(3, 3);
  lcd.print(WiFi.localIP());
  Serial.println(WiFi.localIP());
  conexionwifi();
  DataLogger("¡Conexión WiFi exitosa!", 0);
  DataLogger("Conectando a Ubidots", 0);
  while (WiFi.status() == WL_CONNECTED) {
    LocalCheck();
    SaveLocal();
    if (abs(millis() - currentMillis) > 2000) {
      Serial.print("-");
      desconexionwifi();
      if (ubidots.reconnect()) {
        break;
      }
      currentMillis = millis();
      break;
    }
  }
  DataLogger("¡Conexión a Ubidots exitosa!", 0);
  updateRTC();
  delay(200);
  lcd.setCursor(0, 3);
  lcd.print("                  ");
  conexionwifi();
  ubiOK();
}
