#include "ESPAsyncWebServer.h"

AsyncWebServer server(80);

void SetupServer() {
  server.on("/checkDevice", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument json(1024);
    json["name"] = NAME;
    json["type"] = TYPE;
    json["ssid"] = WIFISSID;
    json["password"] = PASSWORD;
    json["company"] = COMPANY;
    json["time"] = interval_save_local;
    json["NREG"] = NUM_REGISTERS;

    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server.on("/getLastValues", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument json(1024);
    if (DHT_ACTIVE) {
      json["TEMP"] = TEMP;
      json["HUM"] = HUM;
      json["TEMPRANGES"] = String(TEMPMIN) + " - " + String(TEMPMAX);
      json["TEMPCOLOR"] = (TEMP > TEMPMAX || TEMP < TEMPMIN) ? "danger" : "success";
      json["HUMRANGES"] = String(HUMMIN) + " - " + String(HUMMAX);
      json["HUMCOLOR"] = (HUM > HUMMAX || HUM < HUMMIN) ? "danger" : "success";
    }
    if (YL_ACTIVE) {
      json["PSHUM"] = S_HUM;
      json["SHUMRANGES"] = String(S_HUMMIN) + " - " + String(S_HUMMAX);
      json["SHUMCOLOR"] = (S_HUM > S_HUMMAX || S_HUM < S_HUMMIN) ? "danger" : "success";
    }
    if (DS18_ACTIVE) {
      json["PSTEMP"] = S_TEMP;
      json["STEMPRANGES"] = String(S_TEMPMIN) + " - " + String(S_TEMPMAX);
      json["STEMPCOLOR"] = (S_TEMP > S_TEMPMAX || S_TEMP < S_TEMPMIN) ? "danger" : "success";
    }
    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server.on("/getSensors", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    //DynamicJsonDocument json(5000);
    dbGetSensors("select * from sensors");
    if (results.isNull()) {
      NoSD();
    } else {
      Serial.println("hay datos en sensores");
    }
    serializeJson(results, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server.on("/getAllItems", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    DynamicJsonDocument json(500);
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
      json["msjsd"] = "No SD.  ";
    } else {
      json["msjsd"] = "";
    }
    if (ubidots.connected()) {
      json["msjnet"] = "Con Internet";
    } else {
      json["msjnet"] = "Sin internet";
    }

    if (DHT_ACTIVE) {
      json["files"].add("temperatura");
      json["files"].add("humedad");
    }
    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server.on("/createFileFrom", HTTP_GET, [](AsyncWebServerRequest *request) {
    int paramsNr = request->params();
    String fileName = "";
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter *p = request->getParam(i);
      if (p->name() == "fileName") {
        fileName = p->value();
      }
    }
    GetValuesToFile(fileName);
    delay(100);
    File file = SD.open("/data.csv");

    if (file) {
      AsyncWebServerResponse *response = request->beginResponse(file, fileName, "text/xhr", true);
      request->send(response);
    } else {
      AsyncResponseStream *response = request->beginResponseStream("application/json");
      response->addHeader("Access-Control-Allow-Origin", "*");
      DynamicJsonDocument json(1024);
      json["error"] = "error al crear el archivo";
      serializeJson(json, *response);
      response->addHeader("Access-Control-Allow-Origin", "*");
      request->send(response);
    }
    file.close();
  });

  server.on("/getLastNValues", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    //DynamicJsonDocument json(5000);
    first_time_values = true;
    values.clear();
    if (DHT_ACTIVE) {
      GetValuesFromDB("temperatura");
      first_time_values = false;
      GetValuesFromDB("humedad");
      values["TEMPRANGES"] = String(TEMPMIN) + " - " + String(TEMPMAX);
      values["TEMPCOLOR"] = (TEMP >= TEMPMIN && TEMP <= TEMPMAX) ? "success" : "danger";
      values["HUMRANGES"] = String(HUMMIN) + " - " + String(HUMMAX);
      values["HUMCOLOR"] = (HUM >= HUMMIN && HUM <= HUMMAX) ? "success" : "danger";
    }
    if (YL_ACTIVE) {
      GetValuesFromDB("s_h1");
      first_time_values = false;
      GetValuesFromDB("s_h2");
      GetValuesFromDB("s_h3");
      GetValuesFromDB("s_h4");
      values["SHUMRANGES"] = String(S_HUMMIN) + " - " + String(S_HUMMAX);
      values["PSHUM"] = S_HUM;
      values["SHUMCOLOR"] = (S_HUM > S_HUMMAX || S_HUM < S_HUMMIN) ? "success" : "danger";
    }
    if (DS18_ACTIVE) {
      GetValuesFromDB("s_t1");
      first_time_values = false;
      GetValuesFromDB("s_t2");
      GetValuesFromDB("s_t3");
      GetValuesFromDB("s_t4");
      values["STEMPRANGES"] = String(S_TEMPMIN) + " - " + String(S_TEMPMAX);
      values["PSTEMP"] = S_TEMP;
      values["STEMPCOLOR"] = (S_TEMP > S_TEMPMAX || S_TEMP < S_TEMPMIN) ? "success" : "danger";
    }
    values["REGISTERS"] = NUM_REGISTERS;
    serializeJson(values, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server.on("/deleteFrom", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    DynamicJsonDocument json(1024);
    int paramsNr = request->params();
    String sensor_name = "";
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter *p = request->getParam(i);
      if (p->name() == "name") {
        sensor_name = p->value();
      }
    }
    String query = "DELETE FROM registers WHERE sensor_name = '" + sensor_name + "'";
    int st = db_exec(query.c_str());
    json["status"] = st;
    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server.on("/updateDevice", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    DynamicJsonDocument json(512);
    String newName = NAME, newType = TYPE, newSSID = WIFISSID, newPassword = PASSWORD, newCompany = COMPANY, newTime = String(interval_save_local), newNR = NUM_REGISTERS;
    int paramsNr = request->params();
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter *p = request->getParam(i);
      if (p->name() == "newName") {
        newName = p->value();
      }
      if (p->name() == "newType") {
        newType = p->value();
      }
      if (p->name() == "newTime") {
        newTime = p->value();
      }
      if (p->name() == "newSSID") {
        newSSID = p->value();
        newSSID.replace(",", " ");
      }
      if (p->name() == "newPassword") {
        newPassword = p->value();
        newPassword.replace(",", " ");
      }
      if (p->name() == "newNR") {
        newNR = p->value();
      }
    }
    String query = "UPDATE device SET name = '" + newName + "', type = '" + newType + "', network = '" + newSSID + "', password = '" + newPassword + "', save_time = " + newTime + ", registers = '" + newNR + "' where id = 1";
    json["response"] = db_exec(query.c_str());
    json["status"] = "OK";
    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
    delay(2000);
    WiFi.disconnect();
    ESP.restart();
  });

  server.on("/updateSensor", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    DynamicJsonDocument json(5000);
    String newUbiVar, newReadPin, newPinMin, newPinMax, newMin, newMax, newIdeal, active, id;
    int paramsNr = request->params();
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter *p = request->getParam(i);
      if (p->name() == "newUbiVar") {
        newUbiVar = p->value();
      }
      if (p->name() == "newReadPin") {
        newReadPin = p->value();
        Serial.println(newReadPin);
      }
      if (p->name() == "newControlPinMin") {
        newPinMin = p->value();
      }
      if (p->name() == "newControlPinMax") {
        newPinMax = p->value();
      }
      if (p->name() == "newMin") {
        newMin = p->value();
      }
      if (p->name() == "newMax") {
        newMax = p->value();
      }
      if (p->name() == "newIdeal") {
        newIdeal = p->value();
      }
      if (p->name() == "active") {
        active = p->value();
        Serial.println(active);
      }
      if (p->name() == "id") {
        id = p->value();
      }
    }
    String query = "UPDATE sensors SET read_pin = '" + newReadPin + "', pin_min = '" + newPinMin + "', pin_max = '" + newPinMax + "', ubi_var = '" + newUbiVar + "', min = '" + newMin + "', max = '" + newMax + "', ideal = '" + newIdeal + "', active = '" + active + "' where id = " + id + "";
    Serial.println(db_exec(query.c_str()));
    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
    delay(2000);
    ESP.restart();
  });

  server.on("/newSensor", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    DynamicJsonDocument json(5000);
    Serial.println("Nuevo Sensor");
    String newName, newUbiVar, newReadPin, newControlPinMin, newControlPinMax, newMin, newMax, newIdeal, id, active;
    int paramsNr = request->params();
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter *p = request->getParam(i);
      if (p->name() == "newName") {
        newName = p->value();
        Serial.println(p->value());
      }
      if (p->name() == "newUbiVar") {
        newUbiVar = p->value();
        Serial.println(p->value());
      }
      if (p->name() == "newReadPin") {
        newReadPin = p->value();
        Serial.println(p->value());
      }
      if (p->name() == "newControlPinMin") {
        newControlPinMin = p->value();
        Serial.println(p->value());
      }
      if (p->name() == "newControlPinMax") {
        newControlPinMax = p->value();
        Serial.println(p->value());
      }
      if (p->name() == "newUbiVar") {
        newUbiVar = p->value();
        Serial.println(p->value());
      }
      if (p->name() == "newMin") {
        newMin = p->value();
        Serial.println(p->value());
      }
      if (p->name() == "newMax") {
        newMax = p->value();
        Serial.println(p->value());
      }
      if (p->name() == "newIdeal") {
        newIdeal = p->value();
        Serial.println(p->value());
      }
      if (p->name() == "active") {
        active = p->value();
        Serial.println(p->value());
      }
      if (p->name() == "id") {
        id = p->value();
        Serial.println(p->value());
      }
    }
    String query = "INSERT INTO sensors (id, name, read_pin, pin_min, pin_max, ubi_var, min, max, ideal, active) VALUES (" + id + ",'" + newName + "','" + newReadPin + "', '" + newControlPinMin + "' ,'" + newControlPinMax + "' , '" + newUbiVar + "', '" + newMin + "', '" + newMax + "' , '" + newIdeal + "', '" + active + "')";
    json["response"] = db_exec(query.c_str());
    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  bool status;

  // Start server
  server.begin();
}