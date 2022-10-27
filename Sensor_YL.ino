void YLCheck() {
  float sh1 = map(analogRead(YLPIN1), 0, 4095, 100, 0);
  float sh2 = map(analogRead(YLPIN2), 0, 4095, 100, 0);
  float sh3 = map(analogRead(YLPIN3), 0, 4095, 100, 0);
  float sh4 = map(analogRead(YLPIN4), 0, 4095, 100, 0);
  float sh5 = map(analogRead(YLPIN5), 0, 4095, 100, 0);
  float sh6 = map(analogRead(YLPIN6), 0, 4095, 100, 0);
}
void YLLocalSave(String date) {
  if (SaveSensorValue("s_h1", date, String(map(analogRead(YLPIN1), 0, 4095, 100, 0)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h2", date, String(map(analogRead(YLPIN2), 0, 4095, 100, 0)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h3", date, String(map(analogRead(YLPIN3), 0, 4095, 100, 0)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h4", date, String(map(analogRead(YLPIN4), 0, 4095, 100, 0)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h5", date, String(map(analogRead(YLPIN5), 0, 4095, 100, 0)))) {
    NoSD();
  }
  if (SaveSensorValue("s_h6", date, String(map(analogRead(YLPIN6), 0, 4095, 100, 0)))) {
    NoSD();
  }
}