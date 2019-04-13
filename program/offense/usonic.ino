// usonic.ino

int _usonic::getDistance(void) {
  Wire.begin();
  TWBR = 12;
  Wire.requestFrom(8, 1);
  Wire.flush();

  timeOut = millis();
  while (timeOut + 35 >= millis()) {
    if (Wire.available()) {
      data = Wire.read();

      break;
    }
  }

  //受信バッファをクリア
  while (Wire.available()) {
    Wire.read();
  }
  Wire.flush();

  return (int)data;
}