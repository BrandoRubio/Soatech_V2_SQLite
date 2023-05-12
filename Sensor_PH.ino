/*int buf[10], temp11;
float distancia[25];
int longitud_valores1 = sizeof(distancia) / sizeof(int);*/

#include "DFRobot_PH.h"

float voltagePH = 25;
DFRobot_PH ph;

void SetupPH() {
  ph.begin();
  pinMode(PHMINCONTROL, OUTPUT);
  digitalWrite(PHMINCONTROL, VLOW);
  pinMode(PHMAXCONTROL, OUTPUT);
  digitalWrite(PHMAXCONTROL, VLOW);
}
void PHCheck() {
  char cmd[10];
  //temperature = readTemperature();         // read your temperature sensor to execute temperature compensation
  voltagePH = analogRead(PHPIN) / 1024.0 * 5000;  // read the voltage
  PH = ph.readPH(voltagePH, temperature);         // convert voltage to pH with temperature compensation
  Serial.print("pH:");
  Serial.print(PH, 2);
  if (readSerial(cmd)) {
    strupr(cmd);
    if (strstr(cmd, "PH")) {
      ph.calibration(voltagePH, temperature, cmd);  //PH calibration process by Serail CMD
    }
  }
  /*for(int z=0; z<longitud_valores1 ; z++){
      float med = getPH();
      distancia[z]=med;
      delay(10);
    }
  evaluacion_ass(distancia,longitud_valores1);
  int indice_moda = longitud_valores1/2;
  PH = distancia[indice_moda];;*/
  //control para subir PH
  if (PHMINCONTROL && digitalRead(PHMINCONTROL) == VLOW && PH <= PHMIN) {
    DataLogger("Control para subir PH", 0);
    digitalWrite(PHMINCONTROL, VHIGH);
  } else if (PHMINCONTROL && digitalRead(PHMINCONTROL) == VHIGH && PH >= (PHIDEAL - 1)) {
    DataLogger("Apaga control para subir PH", 0);
    digitalWrite(PHMINCONTROL, VLOW);
  }
  //control para bajar PH
  if (PHMAXCONTROL && digitalRead(PHMAXCONTROL) == VLOW && PH >= PHMAX) {
    digitalWrite(PHMAXCONTROL, VHIGH);
    DataLogger("Control para bajar PH", 0);
  } else if (PHMAXCONTROL && digitalRead(PHMAXCONTROL) == VHIGH && PH <= (PHIDEAL + 1)) {
    DataLogger("apaga control para bajar PH", 0);
    digitalWrite(PHMAXCONTROL, VLOW);
  }
  if (alternadorLCD == N_PH) {
    lcd.setCursor(0, 0);
    lcd.print("PH:" + String(PH) + "           ");
    lcd.setCursor(0, 1);
    lcd.print("                    ");
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
  }
}
void PHLocalSave(String date) {
  if (SaveSensorValue("ph", date, (PH == -127 ? "NULL" : String(PH)))) {
    NoSD();
  }
}
void PHUpToUbi(String DATE) {
  if (ubidots.connected()) {
    ubidots.add("ph", PH);
    ubidots.publish(DEVICE_LABEL.c_str());
  } else {
    db_exec(("INSERT INTO registers_no_con (ubi_var, date, value, status) VALUES ('ph', '" + DATE + "','" + PH + "', 'no')").c_str());
  }
}
/*
float getPH() {
  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(PHPIN);
    delay(30);
  }
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp11 = buf[i];
        buf[i] = buf[j];
        buf[j] = temp11;
      }
    }
  }
  long int avgValue = 0;
  for (int i = 2; i < 8; i++)
    avgValue += buf[i];
  float pHVol = ((((float)avgValue) * 5.0) / 4095.0) / 6;
  float PH = (-14.779 * pHVol) + 36.093;
  return (PH);
}
void evaluacion_ass(float* values, int length) {
  int i, j, flag = 1;
  float temp;
  for (i = 1; (i <= length) && flag; i++) {
    flag = 0;
    for (j = 0; j < (length - 1); j++) {
      if (values[j + 1] < values[j]) {
        temp = values[j];
        values[j] = values[j + 1];
        values[j + 1] = temp;
        flag = 1;
      }
    }
  }
}*/
