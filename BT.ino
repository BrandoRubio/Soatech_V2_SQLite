#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial  
SoftwareSerial miBT(25,33);    // Definimos los pines RX y TX del Arduino conectados al Bluetooth

#define DEBUG(a) Serial.println(a);
void SetupBT()
{
  miBT.begin(9600);       // Inicializamos el puerto serie BT (Para Modo AT 2)
}
 
void loopBT()
{
  if (miBT.available()) 
  {
      String data = miBT.readStringUntil('\n');
      DEBUG(data);
  }
    
}