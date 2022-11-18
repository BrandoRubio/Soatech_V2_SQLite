#define VREF 5000    //VREF (mv)
#define ADC_RES 1024 //ADC Resolution

//Single-point calibration Mode=0
//Two-point calibration Mode=1
#define TWO_POINT_CALIBRATION 0

#define READ_TEMP (20) //Current water temperature ℃, Or temperature sensor function

//Single point calibration needs to be filled CAL1_V and CAL1_T
#define CAL1_V (1105) //mv
#define CAL1_T (19)   //℃
//Two-point calibration needs to be filled CAL2_V and CAL2_T
//CAL1 High temperature point, CAL2 Low temperature point
#define CAL2_V (1066) //mv
#define CAL2_T (20)   //℃

const uint16_t DO_Table[41] = {
  14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
  11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
  9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
  7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410
};

uint8_t Temperaturet;
uint16_t ADC_Raw;
uint16_t ADC_Voltage;
uint16_t DO;

void SetupOxy(){
  
}
int16_t readDO(uint32_t voltage_mv, uint8_t temperature_c) {
  #if TWO_POINT_CALIBRATION == 0
    uint16_t V_saturation = (uint32_t)CAL1_V + (uint32_t)35 * temperature_c - (uint32_t)CAL1_T * 35;
    return (voltage_mv * DO_Table[temperature_c] / V_saturation);
  #else
    uint16_t V_saturation = (int16_t)((int8_t)temperature_c - CAL2_T) * ((uint16_t)CAL1_V - CAL2_V) / ((uint8_t)CAL1_T - CAL2_T) + CAL2_V;
    return (voltage_mv * DO_Table[temperature_c] / V_saturation);
  #endif
}

void OxyCheck() {
  Temperaturet = (uint8_t)READ_TEMP;
  ADC_Raw = analogRead(OXYPIN);
  ADC_Voltage = uint32_t(VREF) * ADC_Raw / ADC_RES;
  OXY = readDO(ADC_Voltage, Temperaturet) / 2000;
  if(OXY < (OXYMIN + 1) && !STOXYMIN){
    DataLogger("Control para subir la oxygenacion", 0);
    digitalWrite(OXYMINCONTROL, VHIGH);
    STOXYMIN = true;
    STOXY = false;
  }
  /*if(OXY > (OXYMAX - 1) && !STOXYMAX){
    DataLogger("Control para bajar la oxygenacion", 0);
    digitalWrite(OXYMAXCONTROL, VHIGH);
    STOXYMAX = true;
    STOXY = false;
  }*/
  if (OXY >= (OXYIDEAL - 1) && OXY <= (OXYIDEAL + 1) && !STOXY) {
    DataLogger("Apagamos todos los controles de oxigenacion", 0);
    STOXY = true;
    STOXYMAX = false;
    STOXYMIN = false;
    digitalWrite(OXYMINCONTROL, VLOW);
    digitalWrite(OXYMAXCONTROL, VLOW);
  }
  if (alternadorLCD == N_OXY) {
    lcd.setCursor(0, 0);
    lcd.print("Oxigenacion:" + String(OXY) + "   ");
    lcd.setCursor(0, 1);
    lcd.print("                    ");
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
  }  
}

void OxyLocalSave(String date) {
  if (SaveSensorValue("oxygen", date, (OXY == 0 ? "NULL" : String(OXY)))) {
    NoSD();
  }
}