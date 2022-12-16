#include <LiquidCrystal_I2C.h>  // Library for LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);
int alternadorLCD = 1;
void SetupLCD(bool BT) {
  lcd.init();
  lcd.backlight();
  //Wire.begin(); // Start the I2C

  lcd.setCursor(0, 0);
  lcd.print("   Bienvenido a   ");
  lcd.setCursor(0, 1);
  lcd.print("     Soatech      ");
  /*delay(2000);
  lcd.clear();*/
  if (BT) {
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("      Bluetooth     ");
    lcd.setCursor(0, 1);
    lcd.print("  esperando la red  ");
    lcd.setCursor(0, 2);
    lcd.print("    y contraseña    ");
  }
}
void NoSD() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(" Inserta la tarjeta ");
  lcd.setCursor(0, 2);
  lcd.print("      micro SD     ");
  delay(10000);
  ESP.restart();
}
