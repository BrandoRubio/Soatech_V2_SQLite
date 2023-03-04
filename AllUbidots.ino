#include "UbidotsEsp32Mqtt.h"
String UBIDOTS_TOKEN = "";  //TOKEN de ubidots
String DEVICE_LABEL = "";

Ubidots ubidots("");

void SetupUbidots() {
  ubidots._token = UBIDOTS_TOKEN.c_str();  //se asigna el token del dispositivo extraído de la base de datos
  Connect();
  //ubidots.setDebug(true);
  ubidots.setup();
}

void loopUbidots() {  //se repite el proceso de comprobar si está conectado o no a ubidots
  if (abs(millis() - timer_icons) > 2000) {
      /*if (WiFi.getMode() == WIFI_AP) {
        Serial.println("El punto de acceso está encendido");
      } else if (WiFi.getMode() == WIFI_OFF) {
        Serial.println("El punto de acceso está apagado");
      }*/
    if (!ubidots.connected()) {
      desconexionwifi();
      ubidots.reconnect();
    } else {
      ubiOK();
      checkBackup();
    }
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.softAP(NAME.c_str(), "");
      Connect();
    } else {
      //Serial.println("*");
      WiFi.softAPdisconnect(true);
      conexionwifi();
    }
    timer_icons = millis();
  }
  ubidots.loop();
}