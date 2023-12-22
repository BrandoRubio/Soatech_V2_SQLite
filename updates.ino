#include <HTTPClient.h>
#include <Update.h>
void checkForUpdates() {
  DEVICEID = "Dev_110";
  HTTPClient http;
  http.begin("https://soatech-2a232-default-rtdb.firebaseio.com/devs/" + DEVICEID + "/update/.json");
  int httpCode = http.GET();
  if (httpCode > 0) {
    //Serial.println()
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      if (payload == "true") {
        http.begin("https://soatech-2a232-default-rtdb.firebaseio.com/devs/" + DEVICEID + "/link/.json");
        int linkCode = http.GET();
        if (linkCode > 0) {
          if (linkCode == HTTP_CODE_OK) {
            String response = http.getString();
            int firstQuote = response.indexOf('"');
            int lastQuote = response.lastIndexOf('"');
            String link = response.substring(firstQuote + 1, lastQuote);
            Serial.println(link);
            http.begin(link);
            int updateCode = http.GET();
            if (updateCode == HTTP_CODE_OK) {
              int contentLength = http.getSize();
              Serial.printf("Content Length: %d bytes\n", contentLength);
              WiFiClient* stream = http.getStreamPtr();
              if (Update.begin(contentLength)) {
                Serial.println("Starting OTA update...");
                size_t written = Update.writeStream(*stream);

                if (written == contentLength) {
                  Serial.println("OTA update successful");
                  //ACTUALIZAR BANDERA
                  String dataToPatch = "{\"update\": false}";
                  http.begin("https://soatech-2a232-default-rtdb.firebaseio.com/devs/" + DEVICEID + "/.json");
                  http.addHeader("Content-Type", "application/json");
                  int patchCode = http.PATCH(dataToPatch);
                  if (patchCode > 0) {
                    if (patchCode == HTTP_CODE_OK) {
                      String payload = http.getString();
                      Serial.println("Respuesta PATCH exitosa:");
                      Serial.println(payload);
                    } else {
                      Serial.printf("Error en la solicitud PATCH. Código de estado: %d\n", httpCode);
                    }
                  } else {
                    Serial.println("Error al hacer la solicitud PATCH.");
                  }
                  ////////////////////
                } else {
                  Serial.println("OTA update failed");
                }

                if (Update.end()) {
                  Serial.println("Rebooting...");
                  ESP.restart();
                } else {
                  Serial.println("Update ERROR");
                }
              } else {
                Serial.println("Not enough space to begin OTA");
              }
            } else {
              Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(updateCode).c_str());
              ////////////////////
            }
          }
          ////////////////////
        } else {
          Serial.printf("URL vacía");
        }
      } else {
        //Serial.printf("No hay actualización disponible", httpCode);
      }
    } else {
      Serial.println("Error al hacer la solicitud GET.");
    }
    http.end();
  }
}