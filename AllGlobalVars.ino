//////////////--------------Timers-----------------//////////////
unsigned long timer_up_data;
unsigned long timer_local_check;
unsigned long timer_save_local;
unsigned long timer_icons;
//////////////--------------Intervalos-------------//////////////
const int interval_local_check = 2000;
int interval_save_local = 30000;
//////////////-----Estado alto - bajo (Relays)-----//////////////
bool VHIGH = 1;
bool VLOW = 0;
//////////////--------------DHT11------------------//////////////
bool STDHTMIN, STDHTMAX, STDHT, DHT_ACTIVE = false;
uint8_t DHTPIN1, DHTPIN2, DHTPIN3, DHTPIN4, DHTPIN5, DHTPIN6 = 0;
int N_DHT, TEMPMIN, TEMPFAN, HUMMIN, TEMPMAX, HUMMAX, TEMPIDEAL, HUMIDEAL, TEMPMINCONTROL, TEMPMAXCONTROL, HUMMINCONTROL, HUMMAXCONTROL = 0;
float TEMP, HUM = 0;
//////////////--------------MQ135-----------------//////////////
bool CO2_ACTIVE = false;
uint8_t CO2PIN = 32;
int CO2MIN, CO2MAX, CO2MINCONTROL, CO2IDEAL, CO2MAXCONTROL, N_CO2;
double CO2 = (0);
double NH3 = (0);
/////////////---------------Luminosidad-----------/////////////
bool LUM_ACTIVE = false;
int LUMMIN, LUMMAX, LUMMINCONTROL, LUMIDEAL, LUMMAXCONTROL, N_LUM;
float lux = 0;
//////////////--------------YL-69-----------------//////////////
bool STYLMIN, STYLMAX, STYL, YL_ACTIVE = false;
uint8_t YLPIN1, YLPIN2, YLPIN3, YLPIN4, YLPIN5 = 0;
int NS_H, S_HUMMIN, S_HUMMAX, S_HUMIDEAL, S_HUMMIN_Ctrl, S_HUMMAX_Ctrl = 0;
float S_HUM = 0;
//////////////--------------DS18------------------//////////////
bool STDS18MIN, STDS18MAX, DS18_ACTIVE = false;
uint8_t DS18PIN = 0;
int  NS_T, S_TEMPMIN, S_TEMPMAX, S_TEMPIDEAL, S_TEMPMIN_C, S_TEMPMAX_C = 0;
float S_TEMP = 0;
//////////////--------------OXYGEN----------------//////////////
bool STOXYMIN, STOXYMAX, STOXY, OXY_ACTIVE = false;
uint8_t OXYPIN = 0;
int OXYMIN, OXYMAX, OXYMINCONTROL, OXYIDEAL, OXYMAXCONTROL, N_OXY;
float OXY = 0;
String UBIVAR_OXY;
//////////////--------------CONDUCTIVITY----------//////////////
bool COND_ACTIVE = false;
uint8_t CONDPIN = 0;
int CONDMIN, CONDMAX, /*CONDMINCONTROL, CONDIDEAL, CONDMAXCONTROL,*/ N_COND;
float COND = 0;
//////////////--------------PH--------------------//////////////
bool PH_ACTIVE = false;
uint8_t PHPIN = 0;
int PHMIN, PHMAX, PHMINCONTROL, PHIDEAL, PHMAXCONTROL, N_PH;
float PH = 0;
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
///////////---------variables para sensor de humedad-------///////
const int humedadAire = 4096;
const int humedadAgua = 2400;
const int humedadAire1 = 4096;// segundo sensor marcado
const int humedadAgua1 = 2480;
const int humedadAire2 = 4096;
const int humedadAgua2 = 2360;
const int humedadAire3 = 4096;// segundo sensor marcado
const int humedadAgua3 = 2350;
