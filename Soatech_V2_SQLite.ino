#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <SPI.h>
#include <FS.h>
#include "SPIFFS.h"
#include "SD_MMC.h"
#include "SD.h"
#include <ArduinoJson.h>

/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true
#define MAX_FILE_NAME_LEN 100
#define MAX_STR_LEN 500

char db_file_name[MAX_FILE_NAME_LEN] = "\0";
sqlite3 *db = NULL;
int rc;
//String results = "";
DynamicJsonDocument results(5000);
bool first_time = false;
static int callback(void *data, int argc, char **argv, char **azColName) {
  //Serial.println("Callback");
  int i;
  /*if (first_time) {
    Serial.println((const char *)data);
    for (i = 0; i < argc; i++) {
      if (i)
        Serial.print((char)'\t');
      Serial.printf("%s", azColName[i]);
    }
    Serial.printf("\n");
    first_time = false;
  }*/
  for (i = 0; i < argc; i++) {
    /*if (i)
      Serial.print((char)'\t\t');
    Serial.printf("%s", argv[i] ? argv[i] : "NULL");*/
    results[azColName[i]].add(String(argv[i]));
  }
  //Serial.println("");
  return 0;
}
static int callbackSensors(void *data, int argc, char **argv, char **azColName) {
  int i;
  for (i = 0; i < argc; i++) {
    results[String(argv[1])][azColName[i]].add(String(argv[i]));
  }
  //Serial.println("");
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
  first_time = true;
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
  first_time = true;
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

int input_string(char *str, int max_len) {
  max_len--;
  int ctr = 0;
  str[ctr] = 0;
  while (str[ctr] != '\n') {
    if (Serial.available()) {
      str[ctr] = Serial.read();
      if (str[ctr] >= ' ' && str[ctr] <= '~')
        ctr++;
      if (ctr >= max_len)
        break;
    }
  }
  str[ctr] = 0;
  Serial.println(str);
  return ctr;
}

int input_num() {
  char in[20];
  int ctr = 0;
  in[ctr] = 0;
  while (in[ctr] != '\n') {
    if (Serial.available()) {
      in[ctr] = Serial.read();
      if (in[ctr] >= '0' && in[ctr] <= '9')
        ctr++;
      if (ctr >= sizeof(in))
        break;
    }
  }
  in[ctr] = 0;
  int ret = atoi(in);
  Serial.println(ret);
  return ret;
}

void listDir(fs::FS &fs, const char *dirname) {
  Serial.print(F("Listing directory: "));
  Serial.println(dirname);
  File root = fs.open(dirname);
  if (!root) {
    Serial.println(F("Failed to open directory"));
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print(" Dir : ");
      Serial.println(file.name());
    } else {
      Serial.print(" File: ");
      Serial.print(file.name());
      Serial.print(" Size: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void renameFile(fs::FS &fs, const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println(F("File renamed"));
  } else {
    Serial.println(F("Rename failed"));
  }
}

void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println(F("File deleted"));
  } else {
    Serial.println(F("Delete failed"));
  }
}

enum { CHOICE_OPEN_DB = 1,
       CHOICE_EXEC_SQL,
       CHOICE_EXEC_MULTI_SQL,
       CHOICE_CLOSE_DB,
       CHOICE_LIST_FOLDER,
       CHOICE_RENAME_FILE,
       CHOICE_DELETE_FILE,
       CHOICE_SHOW_FREE_MEM };
int askChoice() {
  Serial.println();
  Serial.println(F("Welcome to SQLite console!!"));
  Serial.println(F("---------------------------"));
  Serial.println();
  Serial.print(F("Database file: "));
  Serial.println(db_file_name);
  Serial.println();
  Serial.println(F("1. Open database"));
  Serial.println(F("2. Execute SQL"));
  Serial.println(F("3. Execute Multiple SQL"));
  Serial.println(F("4. Close database"));
  Serial.println(F("5. List folder contents"));
  Serial.println(F("6. Rename file"));
  Serial.println(F("7. Delete file"));
  Serial.println(F("8. Show free memory"));
  Serial.println();
  Serial.print(F("Enter choice: "));
  return input_num();
}

void displayPrompt(const char *title) {
  Serial.println(F("(prefix /spiffs/ or /sd/ or /sdcard/ for"));
  Serial.println(F(" SPIFFS or SD_SPI or SD_MMC respectively)"));
  Serial.print(F("Enter "));
  Serial.println(title);
}

const char *prefixSPIFFS = "/spiffs/";
const char *prefixSD_SPI = "/sd/";
const char *prefixSD_MMC = "/sdcard/";
fs::FS *ascertainFS(const char *str, int *prefix_len) {
  if (strstr(str, prefixSPIFFS) == str) {
    *prefix_len = strlen(prefixSPIFFS) - 1;
    return &SPIFFS;
  }
  if (strstr(str, prefixSD_SPI) == str) {
    *prefix_len = strlen(prefixSD_SPI) - 1;
    return &SD;
  }
  if (strstr(str, prefixSD_MMC) == str) {
    *prefix_len = strlen(prefixSD_MMC) - 1;
    return &SD_MMC;
  }
  return NULL;
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
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

void appendFile(fs::FS &fs, const char *path, const char *message) {
  //Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    //Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
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