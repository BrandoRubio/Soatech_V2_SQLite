#include "UbidotsEsp32Mqtt.h"
String UBIDOTS_TOKEN = "";  //TOKEN de ubidots
String DEVICE_LABEL = "";

Ubidots ubidots("");

void SetupUbidots() {
  ubidots._token = UBIDOTS_TOKEN.c_str();//se asigna el token del dispositivo extraído de la base de datos
  Connect();
  //ubidots.setDebug(true);
  ubidots.setup();
}

void loopUbidots() {//se repite el proceso de comprobar si está conectado o no a ubidots
  if (abs(millis() - timer_icons) > 2000) {
    if (!ubidots.connected()) {
      desconexionwifi();
      ubidots.reconnect();
    } else {
      ubiOK();
      checkBackup();
    }
    if (WiFi.status() != WL_CONNECTED) {
      Connect();
    } else {
      conexionwifi();
    }
    timer_icons = millis();
  }
  ubidots.loop();
}