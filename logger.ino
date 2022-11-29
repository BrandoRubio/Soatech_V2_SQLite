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
  /*
<<<<<<< HEAD
  DateTime now = rtc.now();
  char buf2[] = "MM-DD-YYYY";
  String Name = now.toString(buf2);
  String nameFolder = "/Logger/"+ Name +".csv"; 

  if(FileExist(SD,nameFolder.c_str())== 1) //SI SE REINICIA EL ESP32, INDICA EL PUNTO DE RESET.
=======*/
  if (sdW.fileExist(SD, nameFolder.c_str()) == 0)  // SI EL ARCHIVO NO EXISTE, CREA UNO NUEVO
  {
    sdW.writeFile(SD, nameFolder.c_str(), "Hora, ,Nivel,Instruccion\r\n ");
    sdW.appendFile(SD, nameFolder.c_str(), text.c_str());
  } else  //SI YA EXISTE, AGREGA LOS DATOS EN EL ARCHIVO
  {
    sdW.appendFile(SD, nameFolder.c_str(), text.c_str());
  }
}
/*
<<<<<<< HEAD
//ESCRITURA EN MEMORIA SD
void dataLog(String instruction, int level)  // Registro de datos en archivo
{ 
  String Niv []= {"[INFO] ","[ERROR] "};
  // INFO - informacion de la tarea, Error -- Error de ejecucion

  DateTime now = rtc.now();
  //BUFFER HORAS-MINUTOS-SEGUNDOS
  char buf1[] = "hh:mm:ss";
  String Time = now.toString(buf1);

  char buf3[] = "MM-DD-YYYY";
  String Name = now.toString(buf3);
  String nameFolder = "/Logger/"+ Name +".csv"; 
  String text = Time + ",:-->,"+ Niv[level] + "," + instruction + "\n";
  
   if(FileExist(SD,nameFolder.c_str())== 0) // SI EL ARCHIVO NO EXISTE, CREA UNO NUEVO
=======*/
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
