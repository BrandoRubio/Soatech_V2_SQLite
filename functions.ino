
byte customChar[] = { //flecha hacia arriba
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00000
};
byte customChar1[] = {//flecha hacia abajo
  B00000,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000
};
byte wifi[] = {//Wifi
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10000,
  B10000,
  B00000
};
byte wifi1[] = {//Wifi
  B00000,
  B00000,
  B00000,
  B00000,
  B01000,
  B11000,
  B11000,
  B00000
};
byte wifi2[] = {//Wifi
  B00000,
  B00000,
  B00000,
  B00100,
  B01100,
  B11100,
  B11100,
  B00000
};
byte wifi3[] = {//Wifi
  B00000,
  B00000,
  B00010,
  B00110,
  B01110,
  B11110,
  B11110,
  B00000
};
byte wifi4[] = {//Wifi
  B00000,
  B00001,
  B00011,
  B00111,
  B01111,
  B11111,
  B11111,
  B00000
};
byte wifi5[] = {//NO_Wifi
  B00000,
  B10001,
  B01010,
  B00100,
  B01010,
  B10001,
  B00000,
  B00000
};
byte arrUp[] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00000,
  B00100
};
byte ubiOKc[] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B01010,
  B01100,
  B01100,
  B01010
};
void conexionwifi() {
  if (WiFi.RSSI() == 0) {
    lcd.setCursor(18, 3);
    lcd.print(" ");
  } else if (WiFi.RSSI() >= -25) {
    lcd.createChar(5, wifi1);
    lcd.setCursor(18, 3);
    lcd.write(5);
  } else if (WiFi.RSSI() >= -50) {
    lcd.createChar(5, wifi2);
    lcd.setCursor(18, 3);
    lcd.write(5);
  } else if (WiFi.RSSI() >= -75) {
    lcd.createChar(5, wifi3);
    lcd.setCursor(18, 3);
    lcd.write(5);
  } else if (WiFi.RSSI() >= -100) {
    lcd.createChar(5, wifi4);
    lcd.setCursor(18, 3);
    lcd.write(5);
  }
}
void desconexionwifi() {
  lcd.createChar(6, wifi5);
  lcd.setCursor(19, 3);
  lcd.write(6);
}
void arrowUp() {
  lcd.createChar(7, arrUp);
  lcd.setCursor(17, 3);
  lcd.write(7);
}
void ubiOK() {
  lcd.createChar(6, ubiOKc);
  lcd.setCursor(19, 3);
  lcd.write(6);
}