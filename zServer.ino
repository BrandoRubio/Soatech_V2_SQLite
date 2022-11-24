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

  server.on("/listDirLoggs", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    listDir(SD, "Logger", 2);
    listDir(SD, "Logging", 2);
    listDir(SD, "UserLogging", 2);
    serializeJson(results, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
    results.clear();
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
    if (OXY_ACTIVE) {
      json["OXY"] = OXY;
      json["OXYRANGES"] = String(OXYMIN) + " - " + String(OXYMAX);
      json["OXYCOLOR"] = (OXY > OXYMAX || OXY < OXYMIN) ? "danger" : "success";
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
    }
    serializeJson(results, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
    results.clear();
  });

  server.on("/getAllItems", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    DynamicJsonDocument json(500);
    /*uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
      json["msjsd"] = "No SD.  ";
    } else {
      json["msjsd"] = "";
    }
    if (ubidots.connected()) {
      json["msjnet"] = "Con Internet";
    } else {
      json["msjnet"] = "Sin internet";
    }*/

    if (DHT_ACTIVE) {
      json["files"].add("temperatura");
      json["files"].add("humedad");
    }
    if (DS18_ACTIVE) {
      json["files"].add("s_t1");
      json["files"].add("s_t2");
      json["files"].add("s_t3");
      json["files"].add("s_t4");
    }
    if (DHT_ACTIVE) {
      json["files"].add("s_h1");
      json["files"].add("s_h2");
      json["files"].add("s_h3");
      json["files"].add("s_h4");
    }
    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server.on("/createFileFrom", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    int paramsNr = request->params();
    String fileName = "";
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter *p = request->getParam(i);
      if (p->name() == "fileName") {
        fileName = p->value();
      }
    }
    GetValuesFromSensor(fileName);
    if (values.isNull()) {
      NoSD();
    }
    serializeJson(values, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
    values.clear();
  });

  server.on("/getFile", HTTP_GET, [](AsyncWebServerRequest *request) {
    int paramsNr = request->params();
    String fileName, fileRoute = "";
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter *p = request->getParam(i);
      if (p->name() == "fileName") {
        fileName = p->value();
        Serial.println(fileName);
      }
      if (p->name() == "fileRoute") {
        fileRoute = p->value();
        Serial.println(fileRoute);
      }
    }
    File file = SD.open(fileRoute);
    if (file) {
      AsyncWebServerResponse *response = request->beginResponse(file, fileName, "text/xhr", true);
      request->send(response);
    } else {
      NoSD();
    }
  });

  server.on("/getLastNValues", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    //DynamicJsonDocument json(5000);
    first_time_values = true;
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
      //values["PSHUM"] = S_HUM;
      values["SHUMCOLOR"] = (S_HUM > S_HUMMAX || S_HUM < S_HUMMIN) ? "danger" : "success";
    }
    if (DS18_ACTIVE) {
      GetValuesFromDB("s_t1");
      first_time_values = false;
      GetValuesFromDB("s_t2");
      GetValuesFromDB("s_t3");
      GetValuesFromDB("s_t4");
      values["STEMPRANGES"] = String(S_TEMPMIN) + " - " + String(S_TEMPMAX);
      //values["PSTEMP"] = S_TEMP;
      values["STEMPCOLOR"] = (S_TEMP > S_TEMPMAX || S_TEMP < S_TEMPMIN) ? "danger" : "success";
    }
    if (OXY_ACTIVE) {
      GetValuesFromDB("oxygen");
      first_time_values = false;
      values["OXYRANGES"] = String(OXYMIN) + " - " + String(OXYMAX);
      values["OXYCOLOR"] = (OXY > OXYMAX || OXY < OXYMIN) ? "danger" : "success";
    }
    values["REGISTERS"] = NUM_REGISTERS;
    serializeJson(values, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
    values.clear();
  });

  server.on("/deleteFrom", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    DynamicJsonDocument json(1024);
    int paramsNr = request->params();
    String ip, sensor_name = "";
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter *p = request->getParam(i);
      if (p->name() == "name") {
        sensor_name = p->value();
      }
      if (p->name() == "ip") {
        ip = p->value();
      }
    }
    String query = "DELETE FROM registers WHERE sensor_name = '" + sensor_name + "'";
    int st = db_exec(query.c_str());
    json["status"] = st;
    DataLogging("QUERY: " + query + " {ip: " + ip + "}", 0);
    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server.on("/updateDevice", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    DynamicJsonDocument json(512);
    String ip;
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
      if (p->name() == "ip") {
        ip = p->value();
      }
    }
    String query = "UPDATE device SET name = '" + newName + "', type = '" + newType + "', network = '" + newSSID + "', password = '" + newPassword + "', save_time = " + newTime + ", registers = '" + newNR + "' where id = 1";
    int r = db_exec(query.c_str());
    if (r) {
      DataLogging("Fallo al actualizar el dispositivo {ip: " + ip + "}", 1);
    } else {
      DataLogging("Dispositivo actualizado {ip: " + ip + "}", 0);
    }
    json["response"] = r;
    json["status"] = "OK";
    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
    DataLogging("Reinicio por actualizacion del dispositivo", 0);
    delay(2000);
    WiFi.disconnect();
    ESP.restart();
  });

  server.on("/updateSensor", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    DynamicJsonDocument json(5000);
    String newUbiVar, newReadPin, newPinMin, newPinMax, newMin, newMax, newIdeal, active, id, ip;
    int paramsNr = request->params();
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter *p = request->getParam(i);
      if (p->name() == "newUbiVar") {
        newUbiVar = p->value();
      }
      if (p->name() == "newReadPin") {
        newReadPin = p->value();
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
      }
      if (p->name() == "id") {
        id = p->value();
      }
      if (p->name() == "ip") {
        ip = p->value();
      }
    }
    String query = "UPDATE sensors SET read_pin = '" + newReadPin + "', pin_min = '" + newPinMin + "', pin_max = '" + newPinMax + "', ubi_var = '" + newUbiVar + "', min = '" + newMin + "', max = '" + newMax + "', ideal = '" + newIdeal + "', active = '" + active + "' where id = " + id + "";
    int r = db_exec(query.c_str());
    if (r) {
      DataLogging("Fallo al actualizar el sensor: " + newUbiVar, 1);
    } else {
      DataLogging("Sensor actualizado: " + newUbiVar, 0);
    }
    json["response"] = r;
    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
    DataLogging("Reinicio por actualizacion del sensor: " + newUbiVar + " {ip: " + ip + "}", 0);
    delay(2000);
    ESP.restart();
  });

  server.on("/newSensor", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    DynamicJsonDocument json(5000);
    String newName, newUbiVar, newReadPin, newControlPinMin, newControlPinMax, newMin, newMax, newIdeal, id, active, ip;
    int paramsNr = request->params();
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter *p = request->getParam(i);
      if (p->name() == "newName") {
        newName = p->value();
      }
      if (p->name() == "newUbiVar") {
        newUbiVar = p->value();
      }
      if (p->name() == "newReadPin") {
        newReadPin = p->value();
      }
      if (p->name() == "newControlPinMin") {
        newControlPinMin = p->value();
      }
      if (p->name() == "newControlPinMax") {
        newControlPinMax = p->value();
      }
      if (p->name() == "newUbiVar") {
        newUbiVar = p->value();
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
      }
      if (p->name() == "id") {
        id = p->value();
      }
      if (p->name() == "ip") {
        ip = p->value();
      }
    }
    String query = "INSERT INTO sensors (id, name, read_pin, pin_min, pin_max, ubi_var, min, max, ideal, active) VALUES (" + id + ",'" + newName + "','" + newReadPin + "', '" + newControlPinMin + "' ,'" + newControlPinMax + "' , '" + newUbiVar + "', '" + newMin + "', '" + newMax + "' , '" + newIdeal + "', '" + active + "')";
    int r = db_exec(query.c_str());
    if (r) {
      DataLogging("Fallo al crear el sensor: " + newUbiVar + " {ip: " + ip + "}", 1);
    } else {
      DataLogging("Sensor creado: " + newUbiVar + " {ip: " + ip + "}", 0);
    }
    json["response"] = r;
    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  bool status;

  // Start server
  server.begin();
}