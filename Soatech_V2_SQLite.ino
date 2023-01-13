#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <FS.h>
#include "SPIFFS.h"
#include "SD_MMC.h"
#include "SD.h"

sqlite3 *db = NULL;
int rc;
#define FORMAT_SPIFFS_IF_FAILED true

DynamicJsonDocument results(7500);
static int callback(void *data, int argc, char **argv, char **azColName) {
  int i;
  for (i = 0; i < argc; i++) {
    results[azColName[i]].add(String(argv[i]));
  }
  return 0;
}
static int callbackSensors(void *data, int argc, char **argv, char **azColName) {
  int i;
  for (i = 0; i < argc; i++) {
    results[String(argv[1])][azColName[i]].add(String(argv[i]));
  }
  return 0;
}
int db_open() {
  if (db != NULL)
    sqlite3_close(db);
  int rc = sqlite3_open("/sd/soatech.db", &db);
  if (rc) {
    NoSD();
    DataLogger("No se pudo abrir la base de datos", 1);
    DataLogger("BD ERROR: " + String(sqlite3_extended_errcode(db)), 1);
    DataLogger("BD ERROR: " + String(sqlite3_errmsg(db)), 1);
    /*Serial.print(F("Can't open database: "));
    Serial.print(sqlite3_extended_errcode(db));*/
    //Serial.print(" ");
    //Serial.println(sqlite3_errmsg(db));
    return rc;
  } else {
    //Serial.println(F("Opened database successfully"));
    DataLogger("Base de datos abierta correctamente", 0);
  }
  return rc;
}

char *zErrMsg = 0;
int db_exec(const char *sql) {
  //Serial.println(sql);
  if (db == NULL) {
    return 0;
  }
  results.clear();
  //long start = micros();
  int rc = sqlite3_exec(db, sql, callback, (void *)"", &zErrMsg);
  if (rc != SQLITE_OK) {
    Serial.print(F("SQL error: "));
    Serial.print(sqlite3_extended_errcode(db));
    Serial.print(" ");
    Serial.println(zErrMsg);
    sqlite3_free(zErrMsg);
  } else
    Serial.print("");
  /*Serial.print(F("Time taken:"));
  Serial.print(micros() - start);
  Serial.println(F(" us"));*/
  return rc;
}
int dbGetSensors(const char *sql) {
  if (db == NULL) {
    return 0;
  }
  results.clear();
  //first_time = true;
  //long start = micros();
  int rc = sqlite3_exec(db, sql, callbackSensors, (void *)"", &zErrMsg);
  if (rc != SQLITE_OK) {
    Serial.print(F("SQL error: "));
    Serial.print(sqlite3_extended_errcode(db));
    Serial.print(" ");
    Serial.println(zErrMsg);
    sqlite3_free(zErrMsg);
  } else
    Serial.print("");
  /*Serial.print(F("Time taken:"));
  Serial.print(micros() - start);
  Serial.println(F(" us"));*/
  return rc;
}

void SetupDB() {
  DataLogger("Iniciando base de datos", 0);
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println(F("Failed to mount file Serial"));
    return;
  }
  sqlite3_initialize();
  db_open();
  DataLogger("Cargando datos del dispositivo", 0);
  loadTheData("Select * from Device");
  DataLogger("Cargando datos de los sensores", 0);
  loadSensorsData("Select * from sensors");
}
void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open("/" + String(dirname));
  //>>>>>>> 5d2c068e0ecc0cace35c9efcc28ae04cbddcc677
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }
  int i = 0;
  File file = root.openNextFile();
  results["DIRS"].add(dirname);
  while (file) {
    if (file.isDirectory()) {
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      //results[dirname].add(file.name());
      String name = file.name();
      name.replace("/" + String(dirname) + "/", "");
      results[dirname][i]["rute"] = String(file.name());
      results[dirname][i]["name"] = name;
      results[dirname][i]["size"] = String(file.size());
      i++;
      /*Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());*/
    }
    file = root.openNextFile();
  }
}
//>>>>>>> 5d2c068e0ecc0cace35c9efcc28ae04cbddcc677
