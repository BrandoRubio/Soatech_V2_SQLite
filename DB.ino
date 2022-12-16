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
    if (String(argv[9]) == "true") {
      activeSesors++;  // = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
      DHT_ACTIVE = true;
      N_DHT = activeSesors;
      String pines = argv[2];
      int cp = pines.indexOf(',');
      int cp2 = pines.indexOf(',', cp + 1);
      int cp3 = pines.indexOf(',', cp2 + 1);
      DHTPIN1 = pines.substring(0, cp).toInt();
      DHTPIN2 = pines.substring(cp + 1, cp2).toInt();
      DHTPIN3 = pines.substring(cp2 + 1, cp3).toInt();
      DHTPIN4 = pines.substring(cp3 + 1).toInt();
      String mins = argv[6];
      int ci = mins.indexOf(',');
      TEMPMIN = mins.substring(0, ci).toInt();
      HUMMIN = mins.substring(ci + 1).toInt();
      String maxs = argv[7];
      ci = maxs.indexOf(',');
      TEMPMAX = maxs.substring(0, ci).toInt();
      HUMMAX = maxs.substring(ci + 1).toInt();
      String ideals = argv[8];
      ci = ideals.indexOf(',');
      TEMPIDEAL = ideals.substring(0, ci).toInt();
      HUMIDEAL = ideals.substring(ci + 1).toInt();
      String minsc = argv[3];
      ci = minsc.indexOf(',');
      TEMPMINCONTROL = minsc.substring(0, ci).toInt();
      HUMMINCONTROL = minsc.substring(ci + 1).toInt();
      String maxsc = argv[4];
      ci = maxsc.indexOf(',');
      TEMPMAXCONTROL = maxsc.substring(0, ci).toInt();
      HUMMAXCONTROL = maxsc.substring(ci + 1).toInt();
    }
  } else if (String(argv[1]) == "Oxygen") {
    if (String(argv[9]) == "true") {
      activeSesors++;  // = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
      OXY_ACTIVE = true;
      //activeSesors = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
      N_OXY = activeSesors;
      OXYPIN = String(argv[2]).toInt();
      OXYMINCONTROL = String(argv[3]).toInt();
      OXYMAXCONTROL = String(argv[4]).toInt();
      OXYMIN = String(argv[6]).toInt();
      OXYMAX = String(argv[7]).toInt();
      OXYIDEAL = String(argv[8]).toInt();
    }
  } else if (String(argv[1]) == "MQ135") {
    MQ135_ACTIVE = (String(argv[9]) == "true") ? true : false;
    activeSesors = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
  } else if (String(argv[1]) == "Conductividad") {
    if (String(argv[9]) == "true") {
      activeSesors++;  // = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
      COND_ACTIVE = true;
      //activeSesors = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
      N_COND = activeSesors;
      CONDPIN = String(argv[2]).toInt();
      CONDMINCONTROL = String(argv[3]).toInt();
      CONDMAXCONTROL = String(argv[4]).toInt();
      CONDMIN = String(argv[6]).toInt();
      CONDMAX = String(argv[7]).toInt();
      CONDIDEAL = String(argv[8]).toInt();
    }
  } else if (String(argv[1]) == "PH") {
    if (String(argv[9]) == "true") {
      activeSesors++;  // = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
      PH_ACTIVE = true;
      //activeSesors = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
      N_PH = activeSesors;
      PHPIN = String(argv[2]).toInt();
      PHMINCONTROL = String(argv[3]).toInt();
      PHMAXCONTROL = String(argv[4]).toInt();
      PHMIN = String(argv[6]).toInt();
      PHMAX = String(argv[7]).toInt();
      PHIDEAL = String(argv[8]).toInt();
    }
  } else if (String(argv[1]) == "Turvidez") {
    TURVIDEZ_ACTIVE = (String(argv[9]) == "true") ? true : false;
    activeSesors = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
  } else if (String(argv[1]) == "Ds18") {
    if (String(argv[9]) == "true") {
      activeSesors++;  // = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
      DS18_ACTIVE = true;
      //activeSesors = DS18_ACTIVE ? activeSesors + 1 : activeSesors;
      NS_T = activeSesors;
      DS18PIN = String(argv[2]).toInt();
      S_TEMPMIN = String(argv[6]).toInt();
      S_TEMPMAX = String(argv[7]).toInt();
      S_TEMPIDEAL = String(argv[8]).toInt();
    }
  } else if (String(argv[1]) == "YL") {
    if (String(argv[9]) == "true") {
      activeSesors++;  // = (String(argv[9]) == "true") ? activeSesors + 1 : activeSesors;
      YL_ACTIVE = true;
      //activeSesors = YL_ACTIVE ? activeSesors + 1 : activeSesors;
      NS_H = activeSesors;
      //Serial.println(NS_H);
      String pines = argv[2];
      int cp = pines.indexOf(',');
      int cp2 = pines.indexOf(',', cp + 1);
      int cp3 = pines.indexOf(',', cp2 + 1);
      int cp4 = pines.indexOf(',', cp3 + 1);
      YLPIN1 = pines.substring(0, cp).toInt();
      YLPIN2 = pines.substring(cp + 1, cp2).toInt();
      YLPIN3 = pines.substring(cp2 + 1, cp3).toInt();
      YLPIN4 = pines.substring(cp3 + 1, cp4).toInt();
      YLPIN5 = pines.substring(cp4 + 1).toInt();
      S_HUMMIN = String(argv[6]).toInt();
      S_HUMMAX = String(argv[7]).toInt();
      S_HUMIDEAL = String(argv[8]).toInt();
      S_HUMMIN_Ctrl = String(argv[3]).toInt();
      S_HUMMAX_Ctrl = String(argv[4]).toInt();
    }
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
DynamicJsonDocument values(15000);
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
  values["sensor"].add(String(argv[1]));
  values["dates"].add(String(argv[0]));
  return 0;
}
int GetValuesFromSensor(String s) {
  c = 0;
  String sql = "SELECT date, value FROM registers WHERE sensor_name = '" + s + "' ORDER BY id DESC LIMIT 500";
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