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
    Serial.print(F("Can't open database: "));
    Serial.print(sqlite3_extended_errcode(db));
    //Serial.print(" ");
    Serial.println(sqlite3_errmsg(db));
    return rc;
  } else
    Serial.println(F("Opened database successfully"));
  return rc;
}

char *zErrMsg = 0;
const char *data = "Salida:";
int db_exec(const char *sql) {
  if (db == NULL) {
    Serial.println("No database open");
    return 0;
  }
  results.clear();
  //long start = micros();
  int rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
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
    Serial.println("No database open");
    return 0;
  }
  results.clear();
  //first_time = true;
  //long start = micros();
  int rc = sqlite3_exec(db, sql, callbackSensors, (void *)data, &zErrMsg);
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
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println(F("Failed to mount file Serial"));
    return;
  }
  SPI.begin();
  SD_MMC.begin();
  SD.begin();
  sqlite3_initialize();
  db_open();
  loadTheData("Select * from Device");
  loadSensorsData("Select * from sensors");
}