#include "UbidotsEsp32Mqtt.h"
const char *UBIDOTS_TOKEN = "BBFF-kmYShN0MgTkqgAFXdybS2SSrYJVzMF";  //TOKEN de ubidots
const char *DEVICE_LABEL = "test-brando";

Ubidots ubidots(UBIDOTS_TOKEN);

void SetupUbidots() {
  Connect();
  ubidots.setup();
}

void loopUbidots() {
  if (abs(millis() - timer_icons) > 2000) {
    if (!ubidots.connected()) {
      ubidots.reconnect();
    } else {
      conexionwifi();
    }
    if (WiFi.status() != WL_CONNECTED) {
      Connect();
    }
  }
  ubidots.loop();
}