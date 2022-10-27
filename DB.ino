static int callbackLoadDeviceData(void *data, int argc, char **argv, char **azColName) {
  NAME = argv[1];
  TYPE = argv[2];
  WIFISSID = argv[3];
  PASSWORD = argv[4];
  COMPANY = argv[5];
  interval_save_local = String(argv[6]).toInt();
  NUM_REGISTERS = argv[7];
  return 0;
}
const char *dataLoad = "Salida:";
int loadTheData(const char *sql) {
  if (db == NULL) {
    Serial.println("No database open");
    return 0;
  }
  int rc = sqlite3_exec(db, sql, callbackLoadDeviceData, (void *)dataLoad, &zErrMsg);
  if (rc != SQLITE_OK) {
    Serial.print(F("SQL error: "));
    Serial.print(sqlite3_extended_errcode(db));
    Serial.print(" ");
    Serial.println(zErrMsg);
    sqlite3_free(zErrMsg);
  } else
    Serial.print("");
  return rc;
}
///Carga de variables de todos los sensores///
static int callbackLoadSensorsData(void *data, int argc, char **argv, char **azColName) {
  if (String(argv[1]) == "DHT11") {
    DHT_ACTIVE = (String(argv[9]) == "true") ? true : false;
    activeSesors = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
    String pines = argv[2];
    int cp = pines.indexOf(',');
    int cp2 = pines.indexOf(',', cp + 1);
    int cp3 = pines.indexOf(',', cp2 + 1);
    DHTPIN1 = pines.substring(0, cp).toInt();
    DHTPIN2 = pines.substring(cp + 1, cp2).toInt();
    DHTPIN3 = pines.substring(cp2 + 1, cp3).toInt();
    DHTPIN4 = pines.substring(cp3 + 1).toInt();
    String mins = argv[6];
    TEMPMIN = mins.substring(0, cp).toInt();
    HUMMIN = mins.substring(cp + 1).toInt();
    String maxs = argv[7];
    TEMPMAX = maxs.substring(0, cp).toInt();
    HUMMAX = maxs.substring(cp + 1).toInt();
    String ideals = argv[8];
    TEMPIDEAL = ideals.substring(0, cp).toInt();
    HUMIDEAL = ideals.substring(cp + 1).toInt();
    String minsc = argv[3];
    TEMPMINCONTROL = minsc.substring(0, cp).toInt();
    HUMMINCONTROL = minsc.substring(cp + 1).toInt();
    String maxsc = argv[4];
    TEMPMAXCONTROL = maxsc.substring(0, cp).toInt();
    HUMMAXCONTROL = maxsc.substring(cp + 1).toInt();
  }
  if (String(argv[1]) == "Oxigenacion") {
    OXYGEN_ACTIVE = (String(argv[9]) == "true") ? true : false;
    activeSesors = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
  }
  if (String(argv[1]) == "MQ135") {
    MQ135_ACTIVE = (String(argv[9]) == "true") ? true : false;
    activeSesors = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
  }
  if (String(argv[1]) == "Conductividad") {
    CONDUCTIVITY_ACTIVE = (String(argv[9]) == "true") ? true : false;
    activeSesors = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
  }
  if (String(argv[1]) == "PH") {
    PH_ACTIVE = (String(argv[9]) == "true") ? true : false;
    activeSesors = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
  }
  if (String(argv[1]) == "Turvidez") {
    TURVIDEZ_ACTIVE = (String(argv[9]) == "true") ? true : false;
    activeSesors = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
  }
  if (String(argv[1]) == "Ds18") {
    DS18_ACTIVE = (String(argv[9]) == "true") ? true : false;
    activeSesors = DS18_ACTIVE ? activeSesors + 1 : activeSesors;
    String pines = argv[2];
    int cp = pines.indexOf(',');
    int cp2 = pines.indexOf(',', cp + 1);
    int cp3 = pines.indexOf(',', cp2 + 1);
    int cp4 = pines.indexOf(',', cp3 + 1);
    int cp5 = pines.indexOf(',', cp4 + 1);
    DS18PIN1 = pines.substring(0, cp).toInt();
    DS18PIN2 = pines.substring(cp + 1, cp2).toInt();
    DS18PIN3 = pines.substring(cp2 + 1, cp3).toInt();
    DS18PIN4 = pines.substring(cp3 + 1, cp4).toInt();
    DS18PIN5 = pines.substring(cp4 + 1, cp5).toInt();
    DS18PIN6 = pines.substring(cp5).toInt();
    S_TEMPMIN = String(argv[6]).toInt();
    S_TEMPMAX = String(argv[7]).toInt();
    S_TEMPIDEAL = String(argv[8]).toInt();
    String minsc = argv[4];
    S_TEMPMIN_C1 = minsc.substring(0, cp).toInt();
    S_TEMPMIN_C2 = minsc.substring(cp + 1, cp2).toInt();
    S_TEMPMIN_C3 = minsc.substring(cp2 + 1, cp3).toInt();
    S_TEMPMIN_C4 = minsc.substring(cp3 + 1, cp4).toInt();
    S_TEMPMIN_C5 = minsc.substring(cp4 + 1, cp5).toInt();
    S_TEMPMIN_C6 = minsc.substring(cp5).toInt();
    String maxsc = argv[4];
    S_TEMPMAX_C1 = maxsc.substring(0, cp).toInt();
    S_TEMPMAX_C2 = maxsc.substring(cp + 1, cp2).toInt();
    S_TEMPMAX_C3 = maxsc.substring(cp2 + 1, cp3).toInt();
    S_TEMPMAX_C4 = maxsc.substring(cp3 + 1, cp4).toInt();
    S_TEMPMAX_C5 = maxsc.substring(cp4 + 1, cp5).toInt();
    S_TEMPMAX_C6 = maxsc.substring(cp5).toInt();
  }
  if (String(argv[1]) == "YL") {
    YL_ACTIVE = (String(argv[9]) == "true") ? true : false;
    activeSesors = YL_ACTIVE ? activeSesors + 1 : activeSesors;
    String pines = argv[2];
    int cp = pines.indexOf(',');
    int cp2 = pines.indexOf(',', cp + 1);
    int cp3 = pines.indexOf(',', cp2 + 1);
    int cp4 = pines.indexOf(',', cp3 + 1);
    int cp5 = pines.indexOf(',', cp4 + 1);
    YLPIN1 = pines.substring(0, cp).toInt();
    YLPIN2 = pines.substring(cp + 1, cp2).toInt();
    YLPIN3 = pines.substring(cp2 + 1, cp3).toInt();
    YLPIN4 = pines.substring(cp3 + 1, cp4).toInt();
    YLPIN5 = pines.substring(cp4 + 1, cp5).toInt();
    YLPIN6 = pines.substring(cp5).toInt();
    S_HUMMIN = String(argv[6]).toInt();
    S_HUMMAX = String(argv[7]).toInt();
    S_HUMIDEAL = String(argv[8]).toInt();
    String minsc = argv[4];
    S_HUMMIN_C1 = minsc.substring(0, cp).toInt();
    S_HUMMIN_C2 = minsc.substring(cp + 1, cp2).toInt();
    S_HUMMIN_C3 = minsc.substring(cp2 + 1, cp3).toInt();
    S_HUMMIN_C4 = minsc.substring(cp3 + 1, cp4).toInt();
    S_HUMMIN_C5 = minsc.substring(cp4 + 1, cp5).toInt();
    S_HUMMIN_C6 = minsc.substring(cp5).toInt();
    String maxsc = argv[4];
    S_HUMMAX_C1 = maxsc.substring(0, cp).toInt();
    S_HUMMAX_C2 = maxsc.substring(cp + 1, cp2).toInt();
    S_HUMMAX_C3 = maxsc.substring(cp2 + 1, cp3).toInt();
    S_HUMMAX_C4 = maxsc.substring(cp3 + 1, cp4).toInt();
    S_HUMMAX_C5 = maxsc.substring(cp4 + 1, cp5).toInt();
    S_HUMMAX_C6 = maxsc.substring(cp5).toInt();
  }
  return 0;
}
const char *sensorsData = "Sensores:";
int loadSensorsData(const char *sql) {
  if (db == NULL) {
    Serial.println("No database open");
    return 0;
  }
  int rc = sqlite3_exec(db, sql, callbackLoadSensorsData, (void *)sensorsData, &zErrMsg);
  if (rc != SQLITE_OK) {
    Serial.print(F("SQL error: "));
    Serial.print(sqlite3_extended_errcode(db));
    Serial.print(" ");
    Serial.println(zErrMsg);
    sqlite3_free(zErrMsg);
  } else
    Serial.print("");
  return rc;
}
static int callbackSaveValue(void *data, int argc, char **argv, char **azColName) {
  return 0;
}
int SaveSensorValue(String n, String d, String v) {
  String sql = "INSERT INTO registers (sensor_name, date, value) VALUES ('" + n + "', '" + d + "','" + v + "')";
  if (db == NULL) {
    Serial.println("No database open");
    return 0;
  }
  int rc = sqlite3_exec(db, sql.c_str(), callbackSaveValue, (void *)sensorsData, &zErrMsg);
  if (rc != SQLITE_OK) {
    Serial.print(F("SQL error: "));
    Serial.print(sqlite3_extended_errcode(db));
    Serial.print(" ");
    Serial.println(zErrMsg);
    sqlite3_free(zErrMsg);
  } else
    Serial.print("");
  return rc;
}
bool first_time_values = true;
DynamicJsonDocument values(10000);
static int callbackValues(void *data, int argc, char **argv, char **azColName) {
  values[argv[0]].add(String(argv[1]));
  if (first_time_values) {
    values["dates"].add(String(argv[2]));
  }
  return 0;
}
int GetValuesFromDB(String s) {
  String sql = "SELECT sensor_name, value, date FROM registers WHERE sensor_name = '" + s + "' ORDER BY id DESC LIMIT " + NUM_REGISTERS;
  if (db == NULL) {
    Serial.println("No database open");
    return 0;
  }
  int rc = sqlite3_exec(db, sql.c_str(), callbackValues, (void *)sensorsData, &zErrMsg);
  if (rc != SQLITE_OK) {
    Serial.print(F("SQL error: "));
    Serial.print(sqlite3_extended_errcode(db));
    Serial.print(" ");
    Serial.println(zErrMsg);
    sqlite3_free(zErrMsg);
  } else
    Serial.print("");
  return rc;
}
int c = 0;
static int callbackNewFile(void *data, int argc, char **argv, char **azColName) {
  String m = String(c++) + "," + String(argv[0]) + "," + String(argv[1]) + "\n";
  appendFile(SD, "/data.csv", m.c_str());
  return 0;
}
int GetValuesToFile(String s) {
  writeFile(SD, "/data.csv", "");
  c = 0;
  String sql = "SELECT date, value FROM registers WHERE sensor_name = '" + s + "' ORDER BY id DESC LIMIT 300";
  if (db == NULL) {
    Serial.println("No database open");
    return 0;
  }
  int rc = sqlite3_exec(db, sql.c_str(), callbackNewFile, (void *)sensorsData, &zErrMsg);
  if (rc != SQLITE_OK) {
    Serial.print(F("SQL error: "));
    Serial.print(sqlite3_extended_errcode(db));
    Serial.print(" ");
    Serial.println(zErrMsg);
    sqlite3_free(zErrMsg);
  } else
    Serial.print("");
  return rc;
}