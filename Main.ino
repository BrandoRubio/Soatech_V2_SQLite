void setup() {
  Serial.begin(115200);
  SetupLCD();
  SetupRTC();
  Verification();
  //SetupBT();
  SetupDB();
  if (activeSesors == 0) {
    lcd.setCursor(0, 2);
    lcd.print(" Sensores inactivos ");
    //dataLog("Sensores inactivos", 0 );
  }
  if (DHT_ACTIVE) {
    //dataLog("Inicializando sensores de temperatura y Humedad", 0 );
    SetupDHT(DHTPIN1, DHTPIN2, DHTPIN3, DHTPIN4);
  }
  if (DS18_ACTIVE) {
    //dataLog("Inicializando sensor de Temperatura en sustrato", 0 );
    SetupDS18();
  }
  if (YL_ACTIVE) {
    //dataLog("Inicializando sensor de Humedad en sustrato", 0 );
    SetupYL();
  }
  if (OXY_ACTIVE) {
    //dataLog("Inicializando sensor de Oxigeno", 0 );
    SetupOxy();
  }
  delay(1000);
  SetupUbidots();
}

void loop() {
  loopUbidots();
  LocalCheck();
  SaveLocal();
  UpToUbidtos();
}

/**Función que invoca los procesos loop de cada sensor**/
/**Asimismo hace su respectivo control dentro de su función**/
void LocalCheck() {
  if (abs(millis() - timer_local_check) > interval_local_check) {
    if (DHT_ACTIVE) {  //Si el sensor de temperatura está activo entonces lee el sensor
      DHT11Check();
    }
    if (YL_ACTIVE) {  //Si el sensor de temperatura está activo entonces lee el sensor
      YLCheck();
    }
    if (DS18_ACTIVE) {  //Si el sensor de temperatura está activo entonces lee el sensor
      DS18Check();
    }
    if (OXY_ACTIVE) {  //Si el sensor de temperatura está activo entonces lee el sensor
      OxyCheck();
    }
    if (alternadorLCD < activeSesors) {
      alternadorLCD++;
    } else {
      alternadorLCD = 1;
    }
    if (ubidots.connected()) {
      ubiOK();
    }
    timer_local_check = millis();
  }
}

/**Función que verifica el tiempo y hace el guardado local de valores en los sensores**/
void SaveLocal() {
  if (abs(millis() - timer_save_local) > interval_save_local) {
    DateTime now = rtc.now();
    if (DHT_ACTIVE) {  //Si el sensor DHT11 está activo entonces guarda su valor
      DHT11LocalSave(now.timestamp(DateTime::TIMESTAMP_FULL));
    }
    if (YL_ACTIVE) {  //Si el sensor de temperatura en sustrato está activo entonces lee el sensor
      YLLocalSave(now.timestamp(DateTime::TIMESTAMP_FULL));
    }
    if (DS18_ACTIVE) {  //Si el sensor de Humedad en sustrato está activo entonces lee el sensor
      DS18LocalSave(now.timestamp(DateTime::TIMESTAMP_FULL));
    }
    if (OXY_ACTIVE) {  //Si el sensor de oxigención está activo entonces lee el sensor
      OxyLocalSave(now.timestamp(DateTime::TIMESTAMP_FULL));
    }
    timer_save_local = millis();
  }
}

/**Función que verifica el tiempo y hace la subida de datos a ubidots en caso de que los sensores estén activos**/
void UpToUbidtos() {
  if (abs(millis() - timer_up_data) > 300000) {
    arrowUp();
    if (DHT_ACTIVE) {  //Si el sensor de temperatura está activo entonces guarda su valor
      DHT11UpToUbi();
    }
    if (YL_ACTIVE) {  //Si el sensor de temperatura está activo entonces lee el sensor
      YLUpToUbi();
    }
    if (DS18_ACTIVE) {  //Si el sensor de temperatura está activo entonces lee el sensor
      DS18UpToUbi();
    }
    timer_up_data = millis();
    lcd.setCursor(17, 3);
    lcd.print(" ");
  }
}
