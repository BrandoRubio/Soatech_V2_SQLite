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
    Serial.println(F("Opened database successfully"));
    DataLogger("Base de datos abierta correctamente", 0);
  }
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
<<<<<<< HEAD
void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
=======

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open("/" + String(dirname));
>>>>>>> 5d2c068e0ecc0cace35c9efcc28ae04cbddcc677
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }
<<<<<<< HEAD

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
=======
  int i = 0;
  File file = root.openNextFile();
  results["DIRS"].add(dirname);
  while (file) {
    if (file.isDirectory()) {
      //Serial.print("  DIR : ");
>>>>>>> 5d2c068e0ecc0cace35c9efcc28ae04cbddcc677
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
<<<<<<< HEAD
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char * path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char * path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}


bool FileExist(fs::FS &fs, const char * path) {
  File file = fs.open(path);
  if (!file) {
    return 0;
  }
  file.close();
  return 1;
}


void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void testFileIO(fs::FS &fs, const char * path) {
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if (file) {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }


  file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++) {
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}
=======
      //results[dirname].add(file.name());
      String name = file.name();
      name.replace("/"+String(dirname)+"/" ,"");
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
>>>>>>> 5d2c068e0ecc0cace35c9efcc28ae04cbddcc677
