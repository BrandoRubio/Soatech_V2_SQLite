void checkBackup() {
  //respaldando = true;
  //if (respaldando) {
  checkNotUploadedValues("SELECT * FROM registers_no_con WHERE status != 'send' ORDER BY id ASC LIMIT 1");
  /*if (uid) {
    Serial.println(uid);
    delay(1000);
  }*/
  //}
}

static int callbackBackup(void *data, int argc, char **argv, char **azColName) {
  String uid = String(argv[0]);
  if (uid) {
    const char *VAR = argv[1];
    Serial.print("^");
    Serial.print(VAR);
    unsigned long DATE = atol(String(argv[2]).c_str()) + 21600;
    float VAL = String(argv[3]).toFloat();
    char *uc = "";
    ubidots.add(VAR, VAL, uc, DATE);
    ubidots.publish(DEVICE_LABEL.c_str());
    if (db_exec(("UPDATE registers_no_con SET status = 'send' WHERE id = " + uid).c_str())) {
      int e = db_exec(("DELETE from registers_no_con WHERE id = " + uid).c_str());
      Serial.print("Registro eliminado: " + uid + "(" + String(e) + ")");
    }
    //return 49;
  } else {
  }
  //int v = db_exec(("DELETE FROM registers_no_con ORDER BY id desc limit 2").c_str());
  /*if (v) {
    //respaldando = false;
    Serial.println("Sincronización detenida.");
  }*/
  return 0;
}
int checkNotUploadedValues(const char *sql) {
  if (db == NULL) {
    return 0;
  }
  int rc = sqlite3_exec(db, sql, callbackBackup, (void *)"", &zErrMsg);
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