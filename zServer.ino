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
    json["utoken"] = UBIDOTS_TOKEN;
    json["uname"] = DEVICE_LABEL;

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
    if (PH_ACTIVE) {
      json["PH"] = PH;
      json["PHRANGES"] = String(PHMIN) + " - " + String(PHMAX);
      json["PHCOLOR"] = (PH > PHMAX || PH < PHMIN) ? "danger" : "success";
    }
    if (COND_ACTIVE) {
      json["COND"] = COND;
      json["CONDRANGES"] = String(CONDMIN) + " - " + String(CONDMAX);
      json["CONDCOLOR"] = (COND > CONDMAX || COND < CONDMIN) ? "danger" : "success";
    }
    if (CO2_ACTIVE) {
      json["CO2"] = CO2;
      json["CO2RANGES"] = String(CO2MIN) + " - " + String(CO2MAX);
      json["CO2COLOR"] = (CO2 > CO2MAX || CO2 < CO2MIN) ? "danger" : "success";
    }
    if (LUM_ACTIVE) {
      json["LUM"] = lux;
      json["LUMRANGES"] = String(LUMMIN) + " - " + String(LUMMAX);
      json["LUMCOLOR"] = (lux > LUMMAX || lux < LUMMIN) ? "danger" : "success";
    }
    if (JSN_ACTIVE) {
      json["JSN"] = JSN;
      json["JSNRANGES"] = String(JSNMIN) + "% - " + String(JSNMAX) + "%";
      json["JSNHEIGHT"] = String(JSNHEIGHT);
      json["JSNCOLOR"] = (JSN > JSNMAX || JSN < JSNMIN) ? "danger" : "success";
    }
    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  server.on("/getNotifications", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument json(1024);
    if (DHT_ACTIVE) {
      json["TEMP"]["id"] = 1;
      json["TEMP"]["text"] = (TEMP > TEMPMAX || TEMP < TEMPMIN) ? "Fuera de rango: " + String(TEMP) + "ºC" : "Estable";
      json["TEMP"]["title"] = "Temperatura";
      json["TEMP"]["value"] = String(round(TEMP * 100) / 100.0) + " °C";
      json["TEMP"]["color"] = (TEMP > TEMPMAX || TEMP < TEMPMIN) ? "#eb445a" : "#2dd36f";
      json["TEMP"]["ranges"] = "de " +String(TEMPMIN) + " a " + String(TEMPMAX);
      json["HUM"]["id"] = 2;
      json["HUM"]["text"] = (HUM > HUMMAX || HUM < HUMMIN) ? "Fuera de rango: " + String(HUM) + "%" : "Estable";
      json["HUM"]["title"] = "Humedad";
      json["HUM"]["value"] = String(round(HUM * 100) / 100.0) + " %";
      json["HUM"]["color"] = (HUM > HUMMAX || HUM < HUMMIN) ? "#eb445a" : "#2dd36f";
      json["HUM"]["ranges"] = "de " + String(HUMMIN) + " a " + String(HUMMAX);
    }
    if (YL_ACTIVE) {
      json["YL"]["id"] = 3;
      json["YL"]["text"] = (S_HUM > S_HUMMAX || S_HUM < S_HUMMIN) ? "Fuera de rango: " + String(S_HUM) + " %" : "Estable";
      json["YL"]["title"] = "Humedad en sutrato";
      json["YL"]["value"] = String(round(S_HUM * 100) / 100.0) + " %";
      json["YL"]["color"] = (S_HUM > S_HUMMAX || S_HUM < S_HUMMIN) ? "#eb445a" : "#2dd36f";
      json["YL"]["ranges"] = "de " +String(S_HUMMIN) + " a " + String(S_HUMMAX);
    }
    if (DS18_ACTIVE) {
      json["DS18"]["id"] = 4;
      json["DS18"]["text"] = (S_TEMP > S_TEMPMAX || S_TEMP < S_TEMPMIN) ? "Fuera de rango: " + String(S_TEMP) + "°C" : "Estable";
      json["DS18"]["title"] = "Temperatura en sutrato";
      json["DS18"]["value"] = String(round(S_TEMP * 100) / 100.0) + "°C";
      json["DS18"]["color"] = (S_TEMP > S_TEMPMAX || S_TEMP < S_TEMPMIN) ? "#eb445a" : "#2dd36f";
      json["DS18"]["ranges"] = "de " +String(S_TEMPMIN) + " a " + String(S_TEMPMAX);
    }
    if (OXY_ACTIVE) {
      json["OXY"]["id"] = 4;
      json["OXY"]["text"] = (OXY > OXYMAX || OXY < OXYMIN) ? "Fuera de rango: " + String(OXY) + "" : "Estable";
      json["OXY"]["title"] = "Oxigenación";
      json["OXY"]["value"] = String(round(OXY * 100) / 100.0) + "";
      json["OXY"]["color"] = (OXY > OXYMAX || OXY < OXYMIN) ? "#eb445a" : "#2dd36f";
      json["OXY"]["ranges"] = "de " +String(OXYMIN) + " a " + String(OXYMAX);
    }
    if (PH_ACTIVE) {
      json["PH"]["id"] = 5;
      json["PH"]["text"] = (PH > PHMAX || PH < PHMIN) ? "Fuera de rango: " + String(PH) + "" : "Estable";
      json["PH"]["title"] = "pH";
      json["PH"]["value"] = String(round(PH * 100) / 100.0) + "";
      json["PH"]["color"] = (PH > PHMAX || PH < PHMIN) ? "#eb445a" : "#2dd36f";
      json["PH"]["ranges"] = "de " +String(PHMIN) + " a " + String(PHMAX);
    }
    if (COND_ACTIVE) {
      json["COND"]["id"] = 6;
      json["COND"]["text"] = (COND > CONDMAX || COND < CONDMIN) ? "Fuera de rango: " + String(COND) + "" : "Estable";
      json["COND"]["title"] = "Conductividad";
      json["COND"]["value"] = String(round(COND * 100) / 100.0) + "";
      json["COND"]["color"] = (COND > CONDMAX || COND < CONDMIN) ? "#eb445a" : "#2dd36f";
      json["COND"]["ranges"] = "de " +String(CONDMIN) + " a " + String(CONDMAX);
    }
    if (CO2_ACTIVE) {
      json["CO2"]["id"] = 7;
      json["CO2"]["text"] = (CO2 > CO2MAX || CO2 < CO2MIN) ? "Fuera de rango: " + String(CO2) + "" : "Estable";
      json["CO2"]["title"] = "CO2";
      json["CO2"]["value"] = String(round(CO2 * 100) / 100.0) + "";
      json["CO2"]["color"] = (CO2 > CO2MAX || CO2 < CO2MIN) ? "#eb445a" : "#2dd36f";
      json["CO2"]["ranges"] = "de " +String(CO2MIN) + " a " + String(CO2MAX);
    }
    if (LUM_ACTIVE) {
      json["LUM"]["id"] = 8;
      json["LUM"]["text"] = (lux > LUMMAX || lux < LUMMIN) ? "Fuera de rango: " + String(lux) + "" : "Estable";
      json["LUM"]["title"] = "Luminosidad";
      json["LUM"]["value"] = String(round(lux * 100) / 100.0) + "";
      json["LUM"]["color"] = (lux > LUMMAX || lux < LUMMIN) ? "#eb445a" : "#2dd36f";
      json["LUM"]["ranges"] = "de " +String(LUMMIN) + " a " + String(LUMMAX);
    }
    if (JSN_ACTIVE) {
      json["JSN"]["id"] = 9;
      json["JSN"]["text"] = (JSN > JSNMAX || JSN < JSNMIN) ? "Fuera de rango: " + String(JSN) + "%" : "Estable";
      json["JSN"]["title"] = "Nivel de agua";
      json["JSN"]["value"] = String(round(JSN * 100) / 100.0) + "%";
      json["JSN"]["color"] = (JSN > JSNMAX || JSN < JSNMIN) ? "#eb445a" : "#2dd36f";
      json["JSN"]["ranges"] = "de " +String(JSNMIN) + " a " + String(JSNMAX);
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

  server.on("/getDBSoatech", HTTP_GET, [](AsyncWebServerRequest *request) {
    File file = SD.open("/soatech.db");
    if (file) {
      AsyncWebServerResponse *response = request->beginResponse(file, "db.db", "text/xhr", true);
      request->send(response);
    } else {
      NoSD();
    }
  });

  server.on("/getLastNValues", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    first_time_values = true;
    if (DHT_ACTIVE) {
      GetValuesFromDB("temperatura");
      first_time_values = false;
      GetValuesFromDB("humedad");
      if (DHTPIN7) {
        GetValuesFromDB("tex");
        GetValuesFromDB("hex");
      }else{
        values["tex"] = 0;
        values["hex"] = 0;
      }
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
    if (PH_ACTIVE) {
      GetValuesFromDB("ph");
      first_time_values = false;
      values["PHRANGES"] = String(PHMIN) + " - " + String(PHMAX);
      values["PHCOLOR"] = (PH > PHMAX || PH < PHMIN) ? "danger" : "success";
    }
    if (COND_ACTIVE) {
      GetValuesFromDB("cond");
      first_time_values = false;
      values["CONDRANGES"] = String(CONDMIN) + " - " + String(CONDMAX);
      values["CONDCOLOR"] = (COND > CONDMAX || COND < CONDMIN) ? "danger" : "success";
    }
    if (CO2_ACTIVE) {
      GetValuesFromDB("CO2");
      first_time_values = false;
      values["CO2RANGES"] = String(CO2MIN) + " - " + String(CO2MAX);
      values["CO2COLOR"] = (CO2 > CO2MAX || CO2 < CO2MIN) ? "danger" : "success";
    }
    if (LUM_ACTIVE) {
      GetValuesFromDB("luminosidad");
      first_time_values = false;
      values["LUMRANGES"] = String(LUMMIN) + " - " + String(LUMMAX);
      values["LUMCOLOR"] = (lux > LUMMAX || lux < LUMMIN) ? "danger" : "success";
    }
    if (JSN_ACTIVE) {
      GetValuesFromDB("JSN");
      first_time_values = false;
      values["JSNHEIGHT"] = String(JSNHEIGHT);
      values["JSNRANGES"] = String(JSNMIN) + "% - " + String(JSNMAX) + "%";
      values["JSNCOLOR"] = (JSN > JSNMAX || JSN < JSNMIN) ? "danger" : "success";
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
    String query = "DELETE FROM registers_no_con WHERE id = 15";
    int st = db_exec(query.c_str());
    serializeJson(json, *response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server.on("/updateDevice", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
    DynamicJsonDocument json(512);
    String ip;
    String newName = NAME, newType = TYPE, newSSID = WIFISSID, newPassword = PASSWORD, newCompany = COMPANY, newTime = String(interval_save_local), newNR = NUM_REGISTERS, utk = UBIDOTS_TOKEN, unm = DEVICE_LABEL;
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
        Serial.println(newSSID);
        newSSID.replace(",", " ");
        Serial.println(newSSID);
      }
      if (p->name() == "newPassword") {
        newPassword = p->value();
        Serial.println(newPassword);
        newPassword.replace(",", " ");
        Serial.println(newPassword);
      }
      if (p->name() == "newNR") {
        newNR = p->value();
      }
      if (p->name() == "ip") {
        ip = p->value();
      }
      if (p->name() == "utk") {
        //utk = p->value();
      }
      if (p->name() == "unm") {
        //unm = p->value();
      }
    }
    String query = "UPDATE device SET name = '" + newName + "', type = '" + newType + "', network = '" + newSSID + "', password = '" + newPassword + "', save_time = " + newTime + ", registers = '" + newNR + "', utoken = '" + utk + "', uname = '" + unm + "' where id = 1";
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
    delay(1000);
    ESP.restart();
  });

  bool status;

  // Start server
  server.begin();
}