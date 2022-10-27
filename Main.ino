void setup() {
  Serial.begin(115200);
  SetupLCD();
  SetupDB();
  SetupRTC();
  //SetupBT();

  if (activeSesors == 0) {
    lcd.setCursor(0, 2);
    lcd.print(" Sensores inactivos ");
  }
  DateTime now = rtc.now();
  String date = now.timestamp(DateTime::TIMESTAMP_FULL);
  String m = date + ",\r\n";
  File f = SD.open("/log.csv");
  if (f) {
    appendFile(SD, "/log.csv", m.c_str());
  } else {
    writeFile(SD, "/log.csv", m.c_str());
  }
  if (DHT_ACTIVE) {
    SetupDHT(DHTPIN1, DHTPIN2, DHTPIN3, DHTPIN4);
  }
  Connect();
  /*Serial.println(NAME);
  Serial.println(TYPE);
  Serial.println(WIFISSID);
  Serial.println(PASSWORD);
  Serial.println(COMPANY);
  Serial.println(interval_save_local);*/
}

void loop() {
  //loopBT();
  LocalCheck();
  SaveLocal();
}

/**Función que invoca los procesos loop de cada sensor**/
/**Asimismo hace su respectivo control dentro de su función**/
void LocalCheck() {
  if (abs(millis() - timer_local_check) > interval_local_check) {
    conexionwifi();
    if (DHT_ACTIVE) {  //Si el sensor de temperatura está activo entonces lee el sensor
      DHT11Check();
    }
    timer_local_check = millis();
  }
}

/**Función que verifica el tiempo y hace el guardado local de valores en los sensores**/
void SaveLocal() {
  if (abs(millis() - timer_save_local) > interval_save_local) {
    DateTime now = rtc.now();
    if (DHT_ACTIVE) {  //Si el sensor de temperatura está activo entonces guarda su valor
      DHT11LocalSave(now.timestamp(DateTime::TIMESTAMP_FULL));
    }
    timer_save_local = millis();
  }
}