//////////////--------------Timers-----------------//////////////
unsigned long timer_up_data;
unsigned long timer_local_check;
unsigned long timer_save_local;
//////////////--------------Intervalos-------------//////////////
const int interval_up_data = 600000;
const int interval_local_check = 2000;
int interval_save_local = 300000;
//////////////--------------DHT11-----------------//////////////
bool DHT_ACTIVE = false;
uint8_t DHTPIN1, DHTPIN2, DHTPIN3, DHTPIN4 = 0;
int TEMPMIN, HUMMIN, TEMPMAX, HUMMAX, TEMPIDEAL, HUMIDEAL, TEMPMINCONTROL, TEMPMAXCONTROL, HUMMINCONTROL, HUMMAXCONTROL = 0;
float TEMP, HUM = 0;
//////////////--------------MQ135-----------------//////////////
bool MQ135_ACTIVE = false;
//////////////--------------YL-69-----------------//////////////
bool YL_ACTIVE = false;
uint8_t YLPIN1, YLPIN2, YLPIN3, YLPIN4, YLPIN5, YLPIN6 = 0;
int S_HUMMIN, S_HUMMAX, S_HUMIDEAL, S_HUMMIN_C1, S_HUMMIN_C2, S_HUMMIN_C3, S_HUMMIN_C4, S_HUMMIN_C5, S_HUMMIN_C6, S_HUMMAX_C1, S_HUMMAX_C2, S_HUMMAX_C3, S_HUMMAX_C4, S_HUMMAX_C5, S_HUMMAX_C6 = 0;
float S_HUM = 0;
//////////////--------------DS18------------------//////////////
bool DS18_ACTIVE = false;
uint8_t DS18PIN1, DS18PIN2, DS18PIN3, DS18PIN4, DS18PIN5, DS18PIN6 = 0;
int S_TEMPMIN, S_TEMPMAX, S_TEMPIDEAL, S_TEMPMIN_C1, S_TEMPMIN_C2, S_TEMPMIN_C3, S_TEMPMIN_C4, S_TEMPMIN_C5, S_TEMPMIN_C6, S_TEMPMAX_C1, S_TEMPMAX_C2, S_TEMPMAX_C3, S_TEMPMAX_C4, S_TEMPMAX_C5, S_TEMPMAX_C6 = 0;
float S_TEMP = 0;
//////////////--------------OXYGEN----------------//////////////
bool OXYGEN_ACTIVE = false;
uint8_t OXYPIN = 0;
int OXYMIN, OXYMAX, OXYMINCONTROL;
float OXY = 0;
String UBIVAR_OXY;
//////////////--------------CONDUCTIVITY----------//////////////
bool CONDUCTIVITY_ACTIVE = false;
//////////////--------------PH--------------------//////////////
bool PH_ACTIVE = false;
//////////////--------------TURVIDEZ--------------//////////////
bool TURVIDEZ_ACTIVE = false;
//////////////--------------Dispositivo------------//////////////
String NAME = "Soatech Device";
String TYPE = "soatech";
String WIFISSID = "INFINITUM4304_2.4";
String PASSWORD = "vxMr7ddEx3";
String COMPANY = "Soatech Enterprise";
String NUM_REGISTERS = "30";
int activeSesors = 0;