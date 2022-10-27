#include "UbidotsEsp32Mqtt.h"
const char *UBIDOTS_TOKEN = "";  // Put here your Ubidots TOKEN

Ubidots ubidots(UBIDOTS_TOKEN);

void SetupUbidots()
{
  ubidots.setup();
  ubidots.reconnect();
}

void loopUbidots()
{
  ubidots.loop();
}