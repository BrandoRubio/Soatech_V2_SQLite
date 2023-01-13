#include <SDW.h>
SDW sdW;
String LoggerName = "logger";
//ESCRITURA EN MEMORIA SD
void DataLogger(String instruction, int level)  // Registro de datos en archivo
{
  String Niv[] = { "[INFO] ", "[ERROR] " };  // INFO - informacion de la tarea, Error -- Error de ejecucion
  DateTime now = rtc.now();                  //BUFFER HORAS-MINUTOS-SEGUNDOS
  char hr[] = "hh:mm:ss";                    //Formato de hora
  String Time = now.toString(hr);            //Hora con formato legible
  char dt[] = "MM-DD-YYYY";                  //Formato de fecha
  LoggerName = now.toString(dt);             //Fecha con formato legible
  String nameFolder = "/Logger/" + LoggerName + ".csv";
  String text = Time + ",:-->," + Niv[level] + "," + instruction + "\r\n";
  if (sdW.fileExist(SD, nameFolder.c_str()) == 0)  // SI EL ARCHIVO NO EXISTE, CREA UNO NUEVO
  {
    sdW.writeFile(SD, nameFolder.c_str(), "Hora, ,Nivel,Instruccion\r\n ");
    sdW.appendFile(SD, nameFolder.c_str(), text.c_str());
  } else  //SI YA EXISTE, AGREGA LOS DATOS EN EL ARCHIVO
  {
    sdW.appendFile(SD, nameFolder.c_str(), text.c_str());
  }
}
void DataLogging(String instruction, int level) {
  String Niv[] = { "[INFO] ", "[ERROR] " };  // INFO - informacion de la tarea, Error -- Error de ejecucion
  DateTime now = rtc.now();                  //BUFFER HORAS-MINUTOS-SEGUNDOS
  char hr[] = "hh:mm:ss";                    //Formato de hora
  String Time = now.toString(hr);            //Hora con formato legible         //Fecha con formato legible
  String nameFolder = "/Logging/" + LoggerName + ".csv";
  String text = Time + ",:-->," + Niv[level] + "," + instruction + "\r\n";

  if (sdW.fileExist(SD, nameFolder.c_str()) == 0)  // SI EL ARCHIVO NO EXISTE, CREA UNO NUEVO
  {
    sdW.writeFile(SD, nameFolder.c_str(), "Hora, ,Nivel,Instruccion\r\n ");
    sdW.appendFile(SD, nameFolder.c_str(), text.c_str());
  } else  //SI YA EXISTE, AGREGA LOS DATOS EN EL ARCHIVO
  {
    sdW.appendFile(SD, nameFolder.c_str(), text.c_str());
  }
}
