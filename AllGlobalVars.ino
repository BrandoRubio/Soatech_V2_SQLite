//////////////--------------Timers-----------------//////////////
unsigned long timer_up_data;
unsigned long timer_local_check;
unsigned long timer_save_local;
unsigned long timer_icons;
//////////////--------------Intervalos-------------//////////////
const int interval_local_check = 2000;
int interval_save_local = 300000;
//////////////-----Estado alto - bajo (Relays)-----//////////////
int VHIGH = 1;
int VLOW = 0;
//////////////--------------DHT11------------------//////////////
bool DHT_ACTIVE = false;
uint8_t DHTPIN1, DHTPIN2, DHTPIN3, DHTPIN4 = 0;
int N_DHT, TEMPMIN, HUMMIN, TEMPMAX, HUMMAX, TEMPIDEAL, HUMIDEAL, TEMPMINCONTROL, TEMPMAXCONTROL, HUMMINCONTROL, HUMMAXCONTROL = 0;
float TEMP, HUM = 0;
//////////////--------------MQ135-----------------//////////////
bool MQ135_ACTIVE = false;
//////////////--------------YL-69-----------------//////////////
bool YL_ACTIVE = false;
uint8_t YLPIN1, YLPIN2, YLPIN3, YLPIN4, YLPIN5 = 0;
int NS_H, S_HUMMIN, S_HUMMAX, S_HUMIDEAL, S_HUMMIN_Ctrl, S_HUMMAX_Ctrl = 0;
float S_HUM = 0;
//////////////--------------DS18------------------//////////////
bool DS18_ACTIVE = false;
uint8_t DS18PIN = 0;
int NS_T, S_TEMPMIN, S_TEMPMAX, S_TEMPIDEAL, S_TEMPMIN_C, S_TEMPMAX_C = 0;
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